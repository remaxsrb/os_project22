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

    printString("==========================UCITAO MEMORIJU=========================\n");

    userMain();

    return 0;
}