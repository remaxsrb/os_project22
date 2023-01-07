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
#include "../tests/printing.hpp"


// Thread Control Block
class TCB
{
public:
    ~TCB() { delete[] stack; }

    uint8 getThreadStatus() const { return this->thread_status; }

    void setThreadStatus(uint8 status) { this->thread_status = status; }

    bool isSysThread() const {return  sysThread;}

    static void outputThreadBody(void*);

    static void idleThreadBody(void*);

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static thread_t createThread( Body body,  void *arg, uint64 *stack, bool runAtCreation);

    static thread_t createOutputThread();
    static thread_t createMainThread();
    //static thread_t createIdleThread();

    int start();

    void *operator new(size_t size) { return __mem_alloc(size); }

    void operator delete(void *ptr) { __mem_free(ptr); }

    static void yield();

    static thread_t running;

    static thread_t output;

    static thread_t main;

    //za potrebu ispravnog rada potrebna je jos jedna IDLE nit koja u pozadini poziva samo dispec i to je njena svrha

    //static thread_t idle;

private:
    TCB(Body body, void *arg, uint64 *stack, bool runAtCreation) :
            body(body),
            arg(arg),
            stack(body != nullptr ? stack : nullptr),
            context({ body!= nullptr ? (uint64) &threadWrapper : 0,
                     body != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(DEFAULT_TIME_SLICE),
            thread_status(CREATED),
            sysThread(false)

    {
        if (body!= nullptr && runAtCreation)
        {
            Scheduler::put(this);
            printString("Stavio sam nit u scheduler ori njenom stvaranju\n");
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

    static int sleep(time_t); //trenutna nit odlaze svoj rad za zadati broj otkucaja

    static int wake(); // budi trenutnu nit ako je uspavana

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
};


#endif //OS_PROJECT2022_TCB_HPP
