//
// Created by remax on 12/21/22.
//

#ifndef OS_PROJECT2022_SLEEPINGTHREADS_HPP
#define OS_PROJECT2022_SLEEPINGTHREADS_HPP


#include "../h/TCB.hpp"
//prvobitno resenje mi je bilo da sam pravim listu uspavanih niti kao skroz novu klasu
//onda sam shvatio da cu imati mnogo manje glavobolje i prostora za gresku ako nasledim sablonsku listu
// i onda samo vodim racuna o uslovima za ubacivanje a ne i o samom ubacivanju

struct SleepingThread{
    thread_t thread;
    time_t relative_time;
    SleepingThread(thread_t _t, time_t _relative_time) : thread(_t), relative_time(_relative_time) {}

    void *operator new(size_t size) { return __mem_alloc(size); }
    void operator delete(void *ptr) { __mem_free(ptr); }

};

class SleepingThreads : public  List<SleepingThread>{

private:
    static thread_t pop();

    static time_t passed;


public:

    static void insert(thread_t thread, time_t _relative_time);

    static void tick();

    static SleepingThreads sleepingThreadsList;

};

#endif //OS_PROJECT2022_SLEEPINGTHREADS_HPP
