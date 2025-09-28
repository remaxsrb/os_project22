//
// Refactored main()
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

int main()
{
    // --- Initialize memory and buffers ---
    MemoryAllocator::initialise_memory();
    Riscv::initBuffers();

    // --- Create main, idle, and output threads ---
    thread_t mainThread   = TCB::createMainThread();
    thread_t idleThread   = TCB::createIdleThread();
    thread_t outputThread = TCB::createOutputThread();

    idleThread->start();
    outputThread->start();

    // --- Now enable supervisor trap handling ---
    Riscv::w_stvec((uint64)&Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);


    printString("main() started\n");

    printString("AVAILABLE MEMORY: ");

    size_t available_memory = MemoryAllocator::mem_get_free_space();
    printInt(available_memory);
    printString("\n");

    // --- Initialize semaphore for user thread ---
    sem_t userSem;
    sem_open(&userSem, 0);

    printString("AVAILABLE MEMORY: ");

    available_memory = MemoryAllocator::mem_get_free_space();
    printInt(available_memory);
    printString("\n");

    Thread* userThread = new Thread(user_wrapper, userSem);
    userThread->start();

    printString("AVAILABLE MEMORY: ");

    available_memory = MemoryAllocator::mem_get_free_space();
    printInt(available_memory);
    printString("\n");
    // --- Wait for user thread to finish ---
    sem_wait(userSem);

    printString("main() ended\n");

    // --- Drain output buffer ---
    while (Riscv::buffOUT->getSize() > 0)
        thread_dispatch();

    // --- Disable interrupts before cleanup ---
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);

    // --- Cleanup ---
    delete mainThread;
    delete idleThread;
    delete outputThread;
    delete userThread;

    delete Riscv::buffIN;
    // Flush output buffer
    while (Riscv::buffOUT->getSize() > 0) {
        printString("main: Flushing output buffer, size: ");
        printInt(Riscv::buffOUT->getSize());
        printString("\n");
        thread_dispatch();
    }
    delete Riscv::buffOUT;

    delete userSem;

    printString("AVAILABLE MEMORY: ");

    available_memory = MemoryAllocator::mem_get_free_space();
    printInt(available_memory);
    printString("\n");

    // --- Stop emulator ---
    Riscv::stopEmulator();

    return 0;
}
