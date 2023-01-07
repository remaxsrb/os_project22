//
// Created by marko on 20.4.22..
//

#include "../h/TCB.hpp"
#include "../h/riscv.hpp"


thread_t TCB::running = nullptr;
thread_t TCB::output = nullptr;
thread_t TCB::main = nullptr;
//thread_t TCB::idle = nullptr;

//imam probleme prilikom pozivanja thread_create u mejnu a da pri tom ne pokrecem samo C_THREAD_API_TEST
// ili CPP_THREAD_API_TEST koristeci console.lib

//pokusaj da proradi mejn sa semaforima i mojom implementacijom koznole se sveo na to da svaka sistemska
// nit u mejnu dobije svoju metodu za stvaranje TCB

uint64 TCB::timeSliceCounter = 0;

thread_t TCB::createThread( Body body, void *arg, uint64 *stack, bool runAtCreation)
{
    return new TCB(body, arg, stack, runAtCreation);
}

//thread_t TCB::createIdleThread()
//{
//    if(!idle)
//    {
//        uint64 *stack = (uint64*)__mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
//        idle = createThread(idleThreadBody, nullptr, stack, false);
//        idle->thread_status=IDLE;
//        idle->sysThread=true;
//    }
//    return idle;
//}

void TCB::idleThreadBody(void*)
{
    while (true) { thread_dispatch(); }
}

thread_t TCB::createOutputThread()
{
    if(!output)
    {
        uint64 *stack = (uint64*)__mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
        output = createThread(outputThreadBody, nullptr, stack, true);
        output->sysThread = true;

    }
    return output;
}

thread_t TCB::createMainThread()
{
    if(!main)
    {
        main = createThread(nullptr, nullptr, nullptr, false);
        running = main;
        main->sysThread = true;

    }
    return main;
}

void TCB::outputThreadBody(void *) {

        while(true){
            while((*((char*)CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT)){
                char c = Riscv::buff->get_char();
                *((char*)CONSOLE_TX_DATA) = c;
            }
        }
}
void TCB::yield()
{
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_DISPATCH));
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{

    if(running==main)
        printString("RUNNING IS MAIN IN DISPATCH\n");
    else if (running==output)
        printString("RUNNING IS OUTPUT IN DISPATCH\n");


    TCB *old = running;
    timeSliceCounter=0;
    if (old->thread_status == RUNNING)
    {
        running->setThreadStatus(READY);
        Scheduler::put(old);
    }

    running = Scheduler::get();

    if (running==output)
        printString("RUNNING IS OUTPUT IN DISPATCH AFTER SCHEDULER::GET\n");

    if (running)
        running->thread_status = RUNNING;

    Riscv::setPriviledge();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{

    Riscv::setPriviledge();
    Riscv::popSppSpie();
    if(running==output)
        printString("RUNNING JE OUTPUT\n");

    running->body(running->arg);
    running->setThreadStatus(FINISHED);
    TCB::yield();
}

int TCB::wake()
{
    if (running->thread_status != SLEEPING) {
        return -1;
    }
    running->thread_status = READY;
    Scheduler::put(running);
    return 0;
}


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


int TCB::sleep(time_t timeout)
{
    if(running->getThreadStatus()!= RUNNING)
        return -1;

    Riscv::sleepingThreads.put(running, timeout);
    dispatch();
    return 0;
}



