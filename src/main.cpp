//
// Created by remax on 12/21/22.
//


#include "../h/riscv.hpp"
#include "../tests/printing.hpp"

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
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    MemoryAllocator::initialise_memory();
    Riscv::initBuffers();

    thread_t mainThread = TCB::createMainThread();
    thread_t outputThread = TCB::createOutputThread();

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    printString("main() started\n");

    sem_t userSem;
    sem_open(&userSem, 0);

    thread_t userThread ;
    thread_create(&userThread, user_wrapper, userSem);

    sem_wait(userSem);

    printString("main() ended\n");

    while (Riscv::buffOUT->getSize() > 0)
        thread_dispatch();

    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);

    delete mainThread;
    delete outputThread;
    delete userThread;
    delete Riscv::buffIN;
    delete Riscv::buffOUT;
    delete userSem;

    return 0;
}