//
// Created by remax on 12/21/22.
//


#include "../h/riscv.hpp"
#include "../tests/printing.hpp"
#include "../h/Buffer.hpp"

extern void userMain();


int main ()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    MemoryAllocator::initialise_memory();

    Riscv::buff = new buffer();

    TCB *outputThread;
    thread_create(&outputThread, &TCB::outputThreadBody, nullptr);

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    TCB* main;
    thread_create(&main, nullptr, nullptr);
    TCB::running = main;
    TCB::running->setThreadStatus(RUNNING);

    userMain();

    //printString("=========== THREAD C API TEST PASSED =============\n");

    //printString("=========== THREAD C++ API TEST PASSED =============\n");

    //printString("=========== CONSUMER PRODUCER API TEST PASSED =============\n");

    //printString("=========== CONSUMER PRODUCER C++ API TEST PASSED =============\n");

    //printString("=========== THREAD SLEEP C API TEST PASSED =============\n");

    //printString("=========== CONSUMER PRODUCER c++ SYNC API TEST PASSED =============\n");

    return 0;
}