//
// Created by remax on 12/20/22.
//

#include "../h/syscall_cpp.hpp"

//imajuci u vidu da konstruktor nadklase Thread kao argument prima pokazivac na void
// potrebna je pomocna struktura koja ce se slati kao argument jer nema smisla pretvarati time_t u void*

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

void PeriodicThread::terminate()
{
    thread_exit();
}

PeriodicThread::PeriodicThread(time_t period) :
        Thread(PeriodicThread::wrapper, new (periodic_struct){this, this->period})
{ this->period = period;}