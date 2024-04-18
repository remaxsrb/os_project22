//
// Created by remax on 12/20/22.
//

#include "../h/_semaphore.hpp"

uint64 _sem::internal_timer = 0;

_sem::_sem( int init) : val(init), closed(false) {}

_sem *_sem::createSemaphore(sem_t *handle,  int init) {
    *handle = new _sem(init);
    return *handle;
}

int _sem::wait()
{

    if(closed)
        return DEAD;

    if(--this->val<0)
        block();
    return 0;

}

int _sem::signal()
{
    if(closed)
        return DEAD;

    if(++val<=0)
        unblock();

    return 0;
}

void _sem::block()
{
    TCB::running->setThreadStatus(WAITING);
    blocked.addLast(TCB::running);
    TCB::dispatch();
}

void _sem::unblock()
{
    thread_t temp = blocked.removeFirst();
    temp->setThreadStatus(READY);
    Scheduler::put(temp);
}

int _sem::close()
{
    if(closed)
        return DEAD;
    closed = true;
    while(blocked.peekLast())
    {
        thread_t temp = blocked.removeFirst();
        temp->setThreadStatus(READY);
        Scheduler::put(temp);
    }
    return 0;

}

int _sem::timedWait(time_t timeout) {

    do {
        int tryWaitResult = tryWait();

        if (tryWaitResult == DEAD) return DEAD;

        if (tryWaitResult == 1) return 0;

        if (internal_timer > timeout) {
            internal_timer = 0;
            return TIMEOUT;
        }


    } while (true);




}

int _sem::tryWait() {

    if(closed)
        return DEAD;

    if(this->val > 0) {
        this->val--;
        return 1;
    } else return 0;
}

