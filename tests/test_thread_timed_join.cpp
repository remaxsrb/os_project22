//
// Created by remax on 10/22/25.
//

#include "test_thread_timed_join.h"
#include "../h/syscall_cpp.hpp"
#include "printing.hpp"

class Worker : public Thread
{

private:
    int id;
    Thread *target;
    time_t timeout;
    bool long_running;

public:
    Worker(bool long_running = false, Thread *target = nullptr, time_t timeout = 0) : Thread(wrapper, this)
    {
        this->target = target;
        this->timeout = timeout;
        this->long_running = long_running;
    }

    static void wrapper(void *arg)
    {
        Worker *t = (Worker *)arg;
        int id = t->getId();

        t->id = id;

        printString("Thread with ID: ");
        printInt(id);
        printString(" started!\n");

        t->run();

        printString("Thread with ID: ");
        printInt(id);
        printString(" finished!\n");
    }

    void run()
    {
        if (long_running)
        {
            for (volatile int i = 0; i < 10000; i++)
                for (volatile int j = 0; j < 5000; j++);

        }
        else
        {
            if (this->target)
            {
                if (this->timeout > 0)
                {
                    printString("Thread ");
                    printInt(id);
                    printString(" waiting for long running thread (timeout = ");
                    printInt(timeout);
                    printString(")...\n");

                    target->join(timeout);

                    printString("Thread ");
                    printInt(id);
                    printString(" done waiting (timeout expired or thread finished)\n");
                }
                else
                {
                    printString("Thread ");
                    printInt(id);
                    printString(" waiting for long running (no timeout)...\n");

                    target->join();

                    printString("Thread ");
                    printInt(id);
                    printString(" finished waiting (long running completed)\n");
                }
            }
        }
    }
};

void time_join()
{
    printString("=== Time Join Test Start ===\n");

    Thread *long_running_thread = new Worker(true);

    Thread *t1 = new Worker(false, long_running_thread, 30);

    Thread *t2 = new Worker(false, long_running_thread);

    long_running_thread->start();
    t1->start();
    t2->start();

    t1->join();
    t2->join();
    long_running_thread->join();

    printString("=== Time Join Test End ===\n");
}