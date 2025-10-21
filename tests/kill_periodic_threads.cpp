//
// Created by remax on 10/21/25.
//

#include "kill_periodic_threads.h"

#include "printing.hpp"
#include "../h/syscall_cpp.hpp"

void kill_periodic_threads() {

    PeriodicThread* pthreadA = new PeriodicThread(20, (char*)"ThreadA with ID: ");
    pthreadA->start();

    PeriodicThread* pthreadB = new PeriodicThread(20, (char*)"ThreadB with ID: ");
    pthreadB->start();

    PeriodicThread* pthreadC = new PeriodicThread(20, (char*)"ThreadC with ID: ");
    pthreadC->start();


    while (true) {

        char c = getc();
        if (c == 'k') {
            printString("GOT CHAR K!\n");

            PeriodicThread* temp = PeriodicThread::createdThreads.removeFirst();
            temp->terminate();
        }
        if (!PeriodicThread::createdThreads.peekFirst()) break;
    }

}
