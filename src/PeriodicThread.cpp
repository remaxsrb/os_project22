//
// Created by remax on 12/20/22.
//

#include "../h/syscall_cpp.hpp"

//posto periodic thread nije zadat sa nekim privatnim poljima mora se napraviti struct
//koji ce drzati argumente tela funkcije i sam pokazivac na kreiranu periodicnu nit

struct periodic_struct
        {
            PeriodicThread *periodic_thread;
            time_t period;
        };



void PeriodicThread::wrapper(void *arg)
{
    periodic_struct *ps = (periodic_struct*)arg;
    while (true) {
        time_sleep(ps->period);
        ps->periodic_thread->periodicActivation();
    }
    delete ps;
}

PeriodicThread::PeriodicThread(time_t period) :
        Thread(PeriodicThread::wrapper, (void*)(new (periodic_struct){this, period}))
{ }