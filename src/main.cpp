//
// Created by remax on 12/21/22.
//


#include "../h/riscv.hpp"
#include "../tests/printing.hpp"
#include "../h/Buffer.hpp"

bool finishedMain = false;
extern void userMain();
void user_wrapper(void*)
{
    printString("UserMain() started.\n\n");
    userMain();
    printString("userMain() finished.\n");
    finishedMain = true;
}


int main ()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    MemoryAllocator::initialise_memory();



    TCB *outputThread;
    thread_create(&outputThread, &TCB::outputThreadBody, nullptr);

    TCB* main;
    thread_create(&main, nullptr, nullptr);

    TCB::running = main;
    TCB::running->setThreadStatus(RUNNING);

    Riscv::buff = new buffer();

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    Riscv::setUserMode(true);

    Console::putc('P');
    Console::putc('A');
    Console::putc('R');
    Console::putc('T');
    Console::putc('I');
    Console::putc('Z');
    Console::putc('A');
    Console::putc('N');



    while (Riscv::buff->getSize() > 0) {
        thread_dispatch();
    }

    //Riscv::userMode=true;



    //userMain();

    //printString("=========== THREAD C API TEST PASSED =============\n");

    //printString("=========== THREAD C++ API TEST PASSED =============\n");

    //printString("=========== CONSUMER PRODUCER API TEST PASSED =============\n");

    //printString("=========== CONSUMER PRODUCER C++ API TEST PASSED =============\n");

    //printString("=========== THREAD SLEEP C API TEST PASSED =============\n");

    //printString("=========== CONSUMER PRODUCER c++ SYNC API TEST PASSED =============\n");

    return 0;
}