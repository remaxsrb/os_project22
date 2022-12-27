//
// Created by remax on 12/21/22.
//

#ifndef OS_PROJECT2022_SLEEPINGTHREADS_HPP
#define OS_PROJECT2022_SLEEPINGTHREADS_HPP


#include "../h/TCB.hpp"


class SleepingThreads{

private:
    struct Elem{
        thread_t data;
        Elem* next;
        time_t relative_time;

        Elem(thread_t data, Elem* next, time_t relative_time) : data(data), next(next), relative_time(relative_time) {}

    };
    Elem *head, *tail;
public:
    SleepingThreads(): head(nullptr), tail(nullptr) {}

    SleepingThreads(const SleepingThreads&) = delete;
    SleepingThreads &operator=(const SleepingThreads&) = delete;

    void put(thread_t thread, time_t timeout);

    void removeAwakenedThreads();


    void tickFirst();
    time_t peekFirstSlice();
};

#endif //OS_PROJECT2022_SLEEPINGTHREADS_HPP
