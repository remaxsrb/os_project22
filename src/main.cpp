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
    //sem_signal((sem_t)sem);
}

int main ()
{

    MemoryAllocator::initialise_memory();
    Riscv::initBuffer();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    thread_t mainThread = TCB::createMainThread();
    thread_t outputThread = TCB::createOutputThread();

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    printString("main() started\n");

    //sem_t userSem = nullptr;
    //sem_open(&userSem, 0);

    thread_t userThread = nullptr;
    thread_create(&userThread, user_wrapper, nullptr);

    //sem_wait(userSem);

    while (Riscv::buffOUT->getSize() > 0)
        thread_dispatch();

    printString("main() ended\n");

    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);

    delete mainThread;
    delete outputThread;
    delete userThread;
    //delete userSem;

    return 0;
}