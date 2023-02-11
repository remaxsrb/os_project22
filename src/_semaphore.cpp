//
// Created by remax on 12/20/22.
//

#include "../h/_semaphore.hpp"

_sem::_sem( int init) : val(init), closed(false) {}

_sem *_sem::createSemaphore(sem_t *handle,  int init) {
    *handle = new _sem(init);
    return *handle;
}

int _sem::wait()
{

    if(closed)
        return -1;

    if(--this->val<0)
        block();
    return 0;

}

int _sem::signal()
{
    if(closed)
        return -1;

    if(++val<=0)
        unblock();

    return 0;
}

void _sem::block()
{
    TCB::running->setThreadStatus(WAITING);
    blocked.addLast(TCB::running);
    thread_dispatch();
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
        return -1;
    closed = true;
    while(blocked.peekLast())
    {
        thread_t temp = blocked.removeFirst();
        temp->setThreadStatus(READY);
        Scheduler::put(temp);
    }
    return 0;

}
