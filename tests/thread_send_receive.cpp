//
// Created by remax on 10/21/25.
//

#include "thread_send_receive.h"
#include "printing.hpp"
#include "../h/syscall_cpp.hpp"

static Thread* threadA;
static Thread* threadB;
static Thread* threadC;

class ThreadA : public Thread {

public:
    ThreadA() : Thread(wrapper, this) {}

    static void wrapper(void* arg) {

        ThreadA* workerA = (ThreadA*)arg;
        workerA->run();
    }

    void run() {
        printString("ThreadA started!\n");
        for (int i = 0; i < 5; i++) {

            threadB->send((char*)"THREAD A -> THREAD B\n");

            threadC->send((char*)"THREAD A -> THREAD C\n");

            char* msg = receive();
            printString(msg);
            printString("\n");
        }
        printString("Worker A finished!\n");
    }
};

class ThreadB : public Thread {

public:
    ThreadB() : Thread(wrapper, this) {}

    static void wrapper(void* arg) {

        ThreadB* workerB = (ThreadB*)arg;
        workerB->run();
    }


    void run() {

        printString("ThreadB started!\n");

        for (int i = 0; i < 5; i++) {

            threadC->send((char*)"THREAD B -> THREAD C#1\n");

            threadC->send((char*)"THREAD B -> THREAD C#2\n");

            char* msg = receive();
            printString(msg);
            printString("\n");
        }
        printString("Worker B finished!\n");
    }
};


class ThreadC : public Thread {

public:
    ThreadC() : Thread(wrapper, this) {}

    static void wrapper(void* arg) {

        ThreadC* workerC = (ThreadC*)arg;
        workerC->run();
    }

    void run() {
        printString("ThreadC started!\n");

        for (int i = 0; i < 5; i++) {

            char* msg = receive();
            printString(msg);
            printString("\n");

            msg = receive();
            printString(msg);
            printString("\n");

            msg = receive();
            printString(msg);
            printString("\n");

            threadB->send((char*)"THREAD C -> THREAD B \n");

            threadA->send((char*)"THREAD C -> THREAD A\n");
        }
        printString("Worker C finished!\n");
    }
};


void test_send_receive () {

    threadA = new ThreadA();
    threadA->start();

    threadB = new ThreadB();
    threadB->start();

    threadC = new ThreadC();
    threadC->start();

    threadA->join();
    threadB->join();
    threadC->join();

}