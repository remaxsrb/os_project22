//
// Created by marko on 20.4.22..
//

#include "../h/TCB.hpp"
#include "../h/riscv.hpp"


TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread( Body body, void *arg, uint64 *stack)
{
   return new TCB(body, arg, stack);

}

void TCB::yield()
{
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_DISPATCH));
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (old->thread_status == RUNNING)
    {
        running->setThreadStatus(READY);
        Scheduler::put(old);
    }

    running = Scheduler::get();

    if (running)
        running->thread_status = RUNNING;

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{

    Riscv::popSppSpie();
    running->body(running->arg);
    running->setThreadStatus(FINISHED);
    TCB::yield();
}

//int TCB::wake()
//{
//    if (this->getThreadStatus() != SLEEPING) {
//        return -1;
//    }
//    this->thread_status = READY;
//    Scheduler::put(this);
//    return 0;
//}


int TCB::start()
{
    if (this->getThreadStatus() != CREATED) {
        return -1;
    }
    this->thread_status = READY;
    Scheduler::put(this);
    return 0;
}

int TCB::exit()
{
    if (running->getThreadStatus() != RUNNING) {
        return -1;
    }
    running->thread_status = FINISHED;
    dispatch();
    return 0;
}

//int TCB::sleep(time_t timeout)
//{
//    if(running->getThreadStatus()!= RUNNING)
//        return -1;
//
//    SleepingThreads::put(running, timeout);
//    dispatch();
//    return 0;
//}

