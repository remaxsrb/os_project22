//
// Created by marko on 20.4.22..
//

#include "../h/Scheduler.hpp"

#include "../h/TCB.hpp"
#include "../tests/printing.hpp"
List<TCB> Scheduler::readyThreadQueue;

TCB *Scheduler::get()
{
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *tcb)
{
    if (tcb->getThreadStatus() == READY)
        readyThreadQueue.addLast(tcb);
}

bool Scheduler::isEmpty()
{
    if(!readyThreadQueue.peekFirst())
        return true;
    return false;
}

int Scheduler::waitingCount() {
    return readyThreadQueue.size();
}
