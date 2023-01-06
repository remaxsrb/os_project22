//
// Created by remax on 12/21/22.
//


#include "../h/riscv.hpp"

extern void userMain();

void user_wrapper(void *sem)
{
    printString("userMain() started\n");
    userMain();
    printString("userMain() finished\n");
    sem_signal((sem_t)sem);
}

int main ()
{

    MemoryAllocator::initialise_memory();
    Riscv::initBuffer();

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    Riscv::setUserMode(true);

    thread_t outputThread = TCB::createOutputThread();

//    thread_t outputThread = nullptr;
//    thread_create(&outputThread, &TCB::outputThreadBody, nullptr);

    thread_t main = TCB::createMainThread();

//    thread_t main = nullptr;
//    thread_create(&main, nullptr, nullptr);
//    TCB::running = main;
//    TCB::running->setThreadStatus(RUNNING);

    printString("main() started\n");

    sem_t userSem = nullptr;
    sem_open(&userSem, 0);
    thread_t userThread  = TCB::createUserThread(user_wrapper, userSem);

//    thread_t userThread = nullptr;
//    thread_create(&userThread, user_wrapper, userSem);

    sem_wait(userSem);
    Riscv::setUserMode(false);


    while (Riscv::buff->getSize() > 0) {
        thread_dispatch();
    }

    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);

    printString("main() ended\n");

    delete main;
    delete outputThread;
    delete userThread;
    delete userSem;

    return 0;
}