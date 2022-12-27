//
// Created by marko on 20.4.22..
//

#include "../h/Scheduler.hpp"
#include "../tests/printing.hpp"
List<TCB> Scheduler::readyThreadQueue;

TCB *Scheduler::get()
{
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *tcb)
{
    printString("===============USAO U SCHEDULER=================\n");

    readyThreadQueue.addLast(tcb);
}
