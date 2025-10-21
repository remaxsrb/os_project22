//
// Created by remax on 10/13/22.
//

#ifndef OS_PROJECT2022_SYSCALL_CPP_HPP
#define OS_PROJECT2022_SYSCALL_CPP_HPP

#include "syscall_c.h"

void *operator new(size_t) ;
void operator delete(void*) ;

class Thread {
public:
    Thread (void (*body)(void*), void *arg);
    virtual ~Thread();

    int start();

    void join();
    void send(char* message);
    char* receive();

    static void dispatch();
    static int sleep(time_t);
    static void wrapper(void*);


protected:
    Thread();
    virtual void run() {}

private:
    thread_t myHandle;
};

class Semaphore {
public:
    Semaphore(unsigned init=1);
    virtual ~Semaphore();

    int wait();
    int signal();

    int timedWait (time_t);
    int tryWait();

private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {

private:
    time_t period;

protected:
    PeriodicThread(time_t period);
    virtual void periodicActivation() {}

public:
    static void wrapper(void*);
    void terminate();

};

class Console {
public:
    static char getc();
    static void putc(char);
};


#endif //OS_PROJECT2022_SYSCALL_CPP_HPP
