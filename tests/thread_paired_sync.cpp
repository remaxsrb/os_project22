//
// Created by remax on 10/22/25.
//

#include "thread_paired_sync.h"
#include "../h/syscall_cpp.hpp"
#include "printing.hpp"

class ThreadPairA : public Thread {
public:
    ThreadPairA() : Thread(wrapper, this) {}

    static void wrapper(void *arg) {
        ThreadPairA *t = (ThreadPairA *) arg;

        t->run();

        printString("Thread A with ID: ");
        printInt(t->getId());
        printString(" finished!\n");
    }


    void run() {
        for (int i = 0; i < 3; i++) {
            printString("A before sync\n");
            this->sync();
            printString("A after sync\n");
        }
    }
};


class ThreadPairB : public Thread {
public:
    ThreadPairB() : Thread(wrapper, this) {
    }

    static void wrapper(void *arg) {
        ThreadPairB *t = (ThreadPairB *) arg;

        t->run();

        printString("Thread B with ID: ");
        printInt(t->getId());
        printString(" finished!\n");
    }

    void run() {
        for (int i = 0; i < 3; i++) {
            printString("B before sync\n");
            this->sync();
            printString("B after sync\n");
        }
    }
};

void test_paired_sync() {
    ThreadPairA *threadA = new ThreadPairA();
    ThreadPairB *threadB = new ThreadPairB();

    Thread::pair(threadA, threadB);

    threadA->start();
    threadB->start();

    threadA->join();
    threadB->join();

    delete threadA;
    delete threadB;
}
