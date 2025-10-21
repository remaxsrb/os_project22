//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../tests/printing.hpp"


using Body = void (*)(void*);


buffer* Riscv::buffIN = nullptr;
buffer* Riscv::buffOUT = nullptr;

void Riscv::stopEmulator() {
    __asm__ volatile("li t0, 0x100000");
    __asm__ volatile("li t1, 0x5555");
    __asm__ volatile("sw t1, 0(t0)");

}


//Ova metoda je neophodna jer ako se bafer direktno u mejnu inicijalizuje
//dolazi do poremecaja u memoriji iz nekog razloga
void Riscv::initBuffers()
{
    buffIN = new buffer();
    buffOUT = new buffer();
}

void Riscv::popSppSpie()
{
    setPriviledge();

    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

uint64 Riscv::syscall(uint64 *args)
{
    uint64  opcode = args[0];
    uint64  return_value = 0;

    switch (opcode) {

        case MEM_ALLOC: {
            size_t volatile size = args[1] * MEM_BLOCK_SIZE;
            return_value = (uint64)__mem_alloc(size);
            break; }
        case MEM_FREE: {
            void *ptr = (void*)args[1];
            return_value = (uint64)__mem_free(ptr);
            break;}
        case MEM_GET_FREE_SPACE: {
            return_value = MemoryAllocator::mem_get_free_space();
            break;
        }
        case MEM_GET_LARGEST_FREE_BLOCK: {
            return_value = MemoryAllocator::mem_get_largest_free_block();
            break;
        }
        case THREAD_CREATE: {
            thread_t *handle = (thread_t*)args[1];
            Body routine = (Body)args[2];
            void *arguments = (void*)args[3];
            uint64  *stack = (uint64*)args[4];

            *handle = TCB::createThread( routine, arguments, stack);

            if(*handle) {
                return_value =0;
            }
            else
                return_value=-1;

            break;
        }
        case THREAD_PREPARE:
        {
            thread_t *handle = (thread_t*)args[1];
            Body routine = (Body)args[2];
            void *arguments = (void*)args[3];
            uint64  *stack = (uint64*)args[4];

            *handle = TCB::createThread( routine, arguments, stack);

            if(*handle)
                return_value =0;
            else
                return_value=-1;
            break;
        }
        case THREAD_EXIT:{
            return_value = TCB::exit();
            break;
        }

        case THREAD_DISPATCH: {

            TCB::dispatch();
            break;
        }

        case THREAD_JOIN: {

            thread_t *handle = (thread_t*)args[1];

            TCB::join(handle);

            break;
        }

        case THREAD_START: {
            thread_t handle = (thread_t)args[1];
            return_value = handle->start();
            break;
        }

        case THREAD_DELETE: {
            thread_t handle = (thread_t)args[1];
            delete handle;
            break;
        }

        case THREAD_ID: {
            return_value = TCB::getThreadID();
            break;
        }

        case THREAD_SEND: {

            thread_t handle = (thread_t)args[1];
            char* message = (char*)args[2];

            handle->spaceAvailable->wait();
            handle->setMessage(message);
            handle->itemAvailable->signal();

            break;
        }

        case THREAD_RECV: {
            thread_t handle = (thread_t)args[1];

            handle->itemAvailable->wait();
            const char* message = handle->getMessage();
            handle->spaceAvailable->signal();

            return_value = (uint64)message;

            break;
        }

        case SEM_OPEN: {

            sem_t *handle = (sem_t*)args[1];
            uint8 init = (uint8)args[2];

            if(_sem::createSemaphore(handle, init))
                return_value=0;
            else
                return_value = -1;

            break;
        }

        case SEM_CLOSE: {

            sem_t handle = (sem_t)args[1];
            return_value = handle->close();

            break;
        }

        case SEM_WAIT: {

            sem_t handle = (sem_t)args[1];
            return_value = handle->wait();

            break;
        }

        case SEM_SIGNAL: {
            sem_t handle = (sem_t)args[1];
            return_value = handle->signal();
            break;
        }

        case SEM_TIMED_WAIT: {
            sem_t handle = (sem_t)args[1];
            time_t  timeout = (time_t)args[2];

            return_value = handle->timedWait(timeout);

            break;

        }

        case SEM_TRY_WAIT: {
            sem_t handle = (sem_t)args[1];

            return_value = handle->tryWait();

            break;

        }

        case TIME_SLEEP: {
            time_t timeout = (time_t)args[1];
            return_value = TCB::sleep(timeout);
            break;
        }

        case GET_C: {

            return_value = buffIN->take();
            break;
        }

        case PUT_C: {
            char c = (char)args[1];
            buffOUT->append(c);
            break;
        }

        default:
            break;
    }


    return return_value;

}

void Riscv::handleSupervisorTrap()
{
    uint64 args[5];

    __asm__ volatile ("mv %0, a0" : "=r" (args[0]));
    __asm__ volatile ("mv %0, a1" : "=r" (args[1]));
    __asm__ volatile ("mv %0, a2" : "=r" (args[2]));
    __asm__ volatile ("mv %0, a3" : "=r" (args[3]));
    __asm__ volatile ("mv %0, a4" : "=r" (args[4]));

    uint64 scause = r_scause();

    if (scause == ECALL_USER || scause == ECALL_SUPER)
    {

        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        w_retval(syscall(args));

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == SOFTWARE)
    {
        //interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);

        TCB::timeSliceCounter++;
        _sem::internal_timer++;

        SleepingThreads::tick();
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {

            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();

            TCB::dispatch();

            w_sstatus(sstatus);
            w_sepc(sepc);
        }

    }
    else if (scause == HARDWARE)
    {

        //interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)

        int irq = plic_claim();
        if (irq == CONSOLE_IRQ) //prekid od tastature
        {

            volatile char status = *((char*)CONSOLE_STATUS);
            while (status & CONSOLE_RX_STATUS_BIT)
            {
                char c = (*(char*)CONSOLE_RX_DATA);

                buffIN->append(c);
                buffOUT->append(c);
                status = *((char*)CONSOLE_STATUS);
            }
        }
        plic_complete(irq);

    }
    else
    {
        uint64 scause = r_scause();
        uint64 stval = r_stval();
        uint64 stvec = r_stvec();
        uint64 sepc = r_sepc();

        printString("scause: ");
        printInt(scause);
        printString("\n");

        printString("stval: ");
        printInt(stval);
        printString("\n");

        printString("stvec: ");
        printInt(stvec);
        printString("\n");

        printString("sepc: ");
        printInt(sepc);
        printString("\n");
    }
}
