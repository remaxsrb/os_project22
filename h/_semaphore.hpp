//
// Created by remax on 12/20/22.
//

#ifndef OS_PROJECT2022__SEMAPHORE_HPP
#define OS_PROJECT2022__SEMAPHORE_HPP

#include "TCB.hpp"
#include "list.hpp"
#include "../lib/mem.h"
#include "semcodes.hpp"

//implementacija semafora je uzeta iz udzbenika, uz gotovo minimalna odstupanja

class _sem {
private:



    static uint64 internal_timer;

    int val;
    List<TCB> blocked;
    bool closed;
    _sem( int init);

    friend class Riscv;

protected:
    void block();
    void unblock();
public:

    void *operator new(size_t size) { return __mem_alloc(size); }

    void operator delete(void *ptr) { __mem_free(ptr); }

    static _sem *createSemaphore(sem_t * handle, int init);

    int wait();
    int signal();

    int timedWait (time_t);
    int tryWait();

    int close();
    int value() const {return  this->val;}
    ~_sem(){close();}
};

#endif //OS_PROJECT2022__SEMAPHORE_HPP
