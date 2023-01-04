//
// Created by os on 10/13/22.
//

#ifndef OS_PROJECT2022_SCHEDULER_HPP
#define OS_PROJECT2022_SCHEDULER_HPP


#include "list.hpp"

class TCB;

class Scheduler
{
private:
    static List<TCB> readyThreadQueue;

public:
    static TCB *get();

    static void put(TCB *tcb);

    static bool isEmpty();

};


#endif //OS_PROJECT2022_SCHEDULER_HPP
