//
// Created by remax on 12/20/22.
//

#include "../h/syscall_cpp.hpp"

//C++ API implementation
Semaphore::Semaphore(unsigned init)
{
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore()
{
    sem_close(myHandle);
}

int Semaphore::wait()
{
    return sem_wait(myHandle);
}

int Semaphore::signal()
{
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t timeout) {
    return sem_timedWait(myHandle, timeout);
}

int Semaphore::tryWait() {
    return sem_tryWait( myHandle);
}
