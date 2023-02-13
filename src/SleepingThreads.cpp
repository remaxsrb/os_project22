//
// Created by remax on 12/22/22.
//

#include "../h/SleepingThreads.hpp"

time_t SleepingThreads::passed = 0;


SleepingThreads SleepingThreads::sleepingThreadsList;

void SleepingThreads::insert(thread_t thread, time_t timeout) {

    SleepingThread *node = new SleepingThread(thread, timeout+passed);
    //dodaje se passed na broj otkucaja koji su prosli radi rasporedjivanja u redu uspavanih niti
    Elem *head = sleepingThreadsList.head;

    if(!head || (node->relative_time < head->data->relative_time))
        sleepingThreadsList.addFirst(node);
    else
    {
        Elem *prev = head;

        for (Elem *curr = head->next; curr; curr = curr->next)
        {
            if(node->relative_time < curr->data->relative_time)
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


    if(sleepingThreadsList.size() > 0)
        passed++;
    else
        passed = 0;

    while (sleepingThreadsList.size() > 0 &&
           sleepingThreadsList.peekFirst()->relative_time <= passed)
    {
        thread_t t = pop();
        t->wake();

    }

}
