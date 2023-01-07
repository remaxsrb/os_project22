//
// Created by remax on 10/13/22.
//

#include "../h/syscall_c.hpp"
#include "../h/opcodes.hpp"
#include "../tests/printing.hpp"



inline void invoke_sys_call(uint64 opcode)
{
    // ovo parce koda se ponavlja u svakom sistemskom pozivu pa sam ga izdvojio
    __asm__ volatile ("mv a0, %0" : : "r" (opcode));
    __asm__ volatile ("ecall");

}

//svaki sistemski poziv povratnu vrednost cuva u registru a0.
//Posto skoro svaki sistemski poziv vraca neku vrednost citanje iz a0 registra je izdvojeno u posebnu metodu

inline uint64 get_return_value()
{
    uint64 volatile ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return ret;
}

void *mem_alloc(size_t size)
{
    size_t blocks = (size % MEM_BLOCK_SIZE == 0) ?
                    size / MEM_BLOCK_SIZE :
                    1 + size / MEM_BLOCK_SIZE;

    __asm__ volatile ("mv a1, %0" : : "r" (blocks));
    invoke_sys_call(MEM_ALLOC);

    return (void*)get_return_value();
}

int mem_free(void *ptr)
{
    if (!ptr)
        return 0;

    __asm__ volatile ("mv a1, %0" : : "r" (ptr));
    invoke_sys_call(MEM_FREE);

    return get_return_value();
}


int thread_create(thread_t *handle, void (*start_routine)(void*), void *arg)
{

    if (!handle)
        return -1;
    if (!start_routine)
        return -2;

    void *stack = mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);

    if (!stack)
        return -3;

    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a3, %0" : : "r" (arg));
    __asm__ volatile("mv a4, %0" : : "r" (stack));

    invoke_sys_call(THREAD_CREATE);

    return get_return_value();
}

int thread_prepare(thread_t *handle, void (*start_routine)(void*), void *arg)
{

    if (!handle)
        return -1;
    if (!start_routine)
        return -2;

    void *stack = mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);

    if (!stack)
        return -3;

    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a3, %0" : : "r" (arg));
    __asm__ volatile("mv a4, %0" : : "r" (stack));
    invoke_sys_call(THREAD_PREPARE);

    return get_return_value();
}


int thread_start(thread_t handle)
{
    if (!handle)
        return -1;

    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    invoke_sys_call(THREAD_START);

    return get_return_value();
}

int thread_exit()
{
    invoke_sys_call(THREAD_EXIT);
    return get_return_value();
}

void thread_dispatch()
{
    invoke_sys_call(THREAD_DISPATCH);
}

int thread_delete(thread_t handle)
{
    if (!handle)
        return -1;

    __asm__ volatile("mv a1, %0" : : "r" (handle));
    invoke_sys_call(THREAD_DELETE);

    return get_return_value();
}

int sem_open (sem_t* handle, unsigned init)
{

    if(!handle)
        return -1;

    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a2, %0" : : "r" (init));
    invoke_sys_call(SEM_OPEN);

    return get_return_value();
}
int sem_close(sem_t handle)
{
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    invoke_sys_call(SEM_CLOSE);

    return get_return_value();
}
int sem_wait(sem_t id)
{
    if(!id)
        return -1;

    __asm__ volatile ("mv a1, %0" : : "r" (id));
    invoke_sys_call(SEM_WAIT);

    return get_return_value();
}
int sem_signal(sem_t id)
{
    if(!id)
        return -1;

    __asm__ volatile ("mv a1, %0" : : "r" (id));
    invoke_sys_call(SEM_SIGNAL);

    return get_return_value();
}


int time_sleep(time_t timeout)
{
    __asm__ volatile ("mv a1, %0" : : "r" (timeout));
    invoke_sys_call(TIME_SLEEP);

    return get_return_value();
}

char getc()
{
    invoke_sys_call(GET_C);

    return (char)get_return_value();

}
void putc(char c)
{
    __asm__ volatile ("mv a1, %0" : : "r" (c));
    invoke_sys_call(PUT_C);

}