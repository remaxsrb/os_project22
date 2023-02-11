//
// Created by remax on 12/22/22.
//

#include "../h/SleepingThreads.hpp"

time_t SleepingThreads::passed = 0;

time_t SleepingThreads::total_passed = 0;

SleepingThreads SleepingThreads::sleepingThreadsList;

void SleepingThreads::insert(thread_t thread, time_t timeout) {

    SleepingThread *node = new SleepingThread(thread, timeout);
    Elem *head = sleepingThreadsList.head;

    if(!head || (node->timeout < head->data->timeout))
        sleepingThreadsList.addFirst(node);
    else
    {
        Elem *prev = head;

        for (Elem *curr = head->next; curr; curr = curr->next)
        {
            if(node->timeout < curr->data->timeout)
            {
                sleepingThreadsList.insertAfter(prev,node);
                return;
            }
            prev = curr;
        }
        sleepingThreadsList.addLast(node);
    }


}

thread_t SleepingThreads::pop() {

    if (sleepingThreadsList.size() <= 0) { return nullptr; }
    SleepingThread *st = sleepingThreadsList.removeFirst();
    thread_t t = st->thread;
    delete st;
    return t;
}

void SleepingThreads::tick() {

    total_passed++;
    if(sleepingThreadsList.size() > 0)
        passed++;
    else
        passed = 0;

    while (sleepingThreadsList.size() > 0 &&
           sleepingThreadsList.peekFirst()->timeout <= passed)
    {
        thread_t t = pop();
        t->wake();

    }

}
