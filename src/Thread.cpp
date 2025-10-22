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

int Thread::getId() {
    return get_thread_id();
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

void Thread::send(char *message) {
    thread_send(myHandle, message);
}

char * Thread::receive() {
    return thread_recv(myHandle);
}

void Thread::wrapper(void *thread)
{
    ((Thread*)thread)->run();
}

void Thread::pair(Thread *t1, Thread *t2)
{
    thread_pair(t1->myHandle, t2->myHandle);
}

void Thread::sync()
{
    thread_sync(this->myHandle);
}
