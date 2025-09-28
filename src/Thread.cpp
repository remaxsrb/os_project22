//
// Created by remax on 12/15/22.
//


#include "../h/syscall_cpp.hpp"

Thread::Thread(void (*body)(void*), void *arg)
{
    myHandle = nullptr;
    thread_prepare(&myHandle, body, arg);
}

Thread::Thread()
{
    myHandle = nullptr;
    thread_prepare(&myHandle, Thread::wrapper, (void*)this);
}

Thread::~Thread()
{
    thread_delete(myHandle);
}

int Thread::start()
{
    return thread_start(myHandle);
}

void Thread::dispatch()
{
    thread_dispatch();
}

int Thread::sleep(time_t time)
{
    return time_sleep(time);
}

void Thread::join()
{
    if(myHandle)
        thread_join(&myHandle);
}

void Thread::wrapper(void *thread)
{
    ((Thread*)thread)->run();
}
