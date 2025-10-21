//
// Created by marko on 20.4.22..
//

#include "../h/TCB.hpp"
#include "../h/riscv.hpp"
#include "../tests/printing.hpp"
#include "../h/_semaphore.hpp"


thread_t TCB::running = nullptr;
thread_t TCB::output = nullptr;
thread_t TCB::main = nullptr;
thread_t TCB::idle = nullptr;


int TCB::globalThreadId = 1;


//imam probleme prilikom pozivanja thread_create u mejnu a da pri tom ne pokrecem samo C_THREAD_API_TEST
// ili CPP_THREAD_API_TEST koristeci console.lib

//pokusaj da proradi mejn sa semaforima i mojom implementacijom koznole se sveo na to da svaka sistemska
// nit u mejnu dobije svoju metodu za stvaranje TCB

uint64 TCB::timeSliceCounter = 0;

thread_t TCB::createThread( Body body, void *arg, uint64 *stack) {
    thread_t t= new TCB(body, arg, stack);
    t->threadID = globalThreadId++;
    _sem::createSemaphore(&t->spaceAvailable, 10);
    _sem::createSemaphore(&t->itemAvailable, 0);

    return  t;
}
void TCB::outputThreadBody(void *) {

    while(true){
        while((*((char*)CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT)){
            char c = Riscv::buffOUT->take();
            *((char*)CONSOLE_TX_DATA) = c;
        }
    }
}

void TCB::idleThreadBody(void *) {

    while(true){
        thread_dispatch();
    }
}

thread_t TCB::createOutputThread()
{
    if(!output)
    {
        uint64 *stack = (uint64*)__mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
        output = createThread(outputThreadBody, nullptr, stack);
        output->sysThread = true;
    }
    return output;
}

thread_t TCB::createIdleThread()
{
    if(!idle)
    {
        uint64 *stack = (uint64*)__mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
        idle = createThread(idleThreadBody, nullptr, stack);
        idle->sysThread = true;
    }
    return idle;
}

thread_t TCB::createMainThread()
{
    if(!main)
    {
        main = createThread(nullptr, nullptr, nullptr);
        main->sysThread = true;
        running = main;
    }
    return main;
}

void TCB::yield()
{
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_DISPATCH));
    __asm__ volatile ("ecall");
}

char * TCB::getMessage() {

    char **ptr = this->messageQueue.removeFirst();
    char *msg = *ptr;
    delete ptr;
    return msg;
}

void TCB::setMessage(char *msg) {
    char** ptr_msg = new char*(msg);
    this->messageQueue.addLast(ptr_msg);
}

void TCB::dispatch() {
    timeSliceCounter = 0;

    TCB *old = running;

    if (old->thread_status == RUNNING) {
        old->thread_status = READY;
        Scheduler::put(old);
    }
    running = Scheduler::get();

    if (!running) running = idle;

    running->thread_status = RUNNING;

    Riscv::setPriviledge(); // <===== set execution privileges based on if thread is sys thread or not
    contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    exit();

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
    if (running->getThreadStatus() != RUNNING)   return -1;

    running->thread_status = FINISHED;

    yield();

    return -1; // Should not reach here
}

int TCB::sleep(time_t timeout)
{
    if(running->thread_status != RUNNING)
       return -1;

    running->thread_status=SLEEPING;
    SleepingThreads::insert(running, timeout);
    dispatch();

    return 0;
}

int TCB::wake()
{
    if(thread_status != SLEEPING)
        return -1;

    thread_status = READY;
    Scheduler::put(this);
    return 0;

}

void TCB::join(thread_t *handle)
{
    while((*handle)->getThreadStatus() != FINISHED)
        dispatch();

}

int TCB::getThreadID() {
    return running->threadID;
}



