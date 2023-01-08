//
// Created by marko on 20.4.22..
//

#ifndef OS_PROJECT2022_TCB_HPP
#define OS_PROJECT2022_TCB_HPP


#include "../lib/hw.h"
#include "Scheduler.hpp"
#include "threadcodes.hpp"
#include "../lib/mem.h"
#include "syscall_cpp.hpp"


// Thread Control Block
class TCB
{
public:
    ~TCB() { delete[] stack; }

    uint8 getThreadStatus() const { return this->thread_status; }

    void setThreadStatus(uint8 status) { this->thread_status = status; }

    bool isSysThread() const {return  sysThread;}

    static void outputThreadBody(void*);


    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static thread_t createThread( Body body,  void *arg, uint64 *stack, bool runAtCreation);

    static thread_t createOutputThread();
    static thread_t createMainThread();

    int start();

    void *operator new(size_t size) { return __mem_alloc(size); }

    void operator delete(void *ptr) { __mem_free(ptr); }

    static void yield();

    static thread_t running;

    static thread_t output;

    static thread_t main;

private:
    TCB(Body body, void *arg, uint64 *stack, bool runAtCreation) :
            body(body),
            arg(arg),
            stack(body != nullptr ? stack : nullptr),
            context({ body!= nullptr ? (uint64) &threadWrapper : 0,
                     body != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
                    }),
            timeSlice(DEFAULT_TIME_SLICE),
            thread_status(body!= nullptr ? CREATED : RUNNING),
            sysThread(false)
    {
        if (body!= nullptr && runAtCreation)
        {
            Scheduler::put(this);
            this->thread_status=READY;
        }
    }

private:

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    uint8 thread_status;

    bool sysThread; // fleg da svaka nit zna da li je sistemska ili nije

    /*nit ce imati maksimalno sest/sedam stanja te ce 8 bita biti vise nego dovoljno da se ona cuvaju
     * mozda bi bilo pametnije da se ovo uradi preko enuma na kraju
     *
     * 0 - CREATED
     * 1 - RUNNING
     * 2 - READY
     * 3 - WAITING
     * 4 - SLEEPING
     * 5 - IDLE
     * 6 - FINISHED
     * */


    friend class Riscv;

    friend class Semaphore;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static int exit(); //gasenje trenutne niti

    static uint64 timeSliceCounter;

};


#endif //OS_PROJECT2022_TCB_HPP
