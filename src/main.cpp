//
// Created by remax on 12/21/22.
//


#include "../h/riscv.hpp"
#include "../tests/printing.hpp"

extern void userMain();




int main ()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    MemoryAllocator::initialise_memory();

    TCB* main;
    thread_create(&main, nullptr, nullptr);
    TCB::running = main;
    TCB::running->setThreadStatus(RUNNING);

    userMain();

    printString("=========== THREAD C API TEST PASSED =============\n");

    return 0;
}