//
// Created by remax on 10/13/22.
//

#ifndef OS_PROJECT2022_SYSCALL_C_H
#define OS_PROJECT2022_SYSCALL_C_H

#include "../lib/hw.h"

void *mem_alloc(size_t size);
int mem_free(void*);
size_t mem_get_free_space();
size_t mem_get_largest_free_block();

class TCB;
typedef TCB *thread_t;
int thread_create(
        thread_t *handle,
        void(*start_routine)(void*),
        void *arg
);
int thread_prepare(
        thread_t *handle,
        void(*start_routine)(void*),
        void *arg
);


int thread_start(thread_t handle);
int thread_exit();
void thread_dispatch();
int thread_delete(thread_t handle);
void thread_join(thread_t *handle);
void thread_send(thread_t handle, char* message);
char* thread_recv(thread_t handle);
int get_thread_id();

class _sem;
typedef _sem *sem_t;
int sem_open(
        sem_t *handle,
        unsigned init
);

int sem_timedWait (sem_t id, time_t);
int sem_tryWait(sem_t id);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);



typedef unsigned long time_t;
int time_sleep(time_t);

const int EOF = -1;
char getc();
void putc(char);


#endif //OS_PROJECT2022_SYSCALL_C_H
