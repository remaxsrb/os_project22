//
// Created by remax on 12/20/22.
//

#include "../h/syscall_cpp.hpp"
#include "../h/TCB.hpp"
#include "../tests/printing.hpp"

//imajuci u vidu da konstruktor nadklase Thread kao argument prima pokazivac na void
// potrebna je pomocna struktura koja ce se slati kao argument jer nema smisla pretvarati time_t u void*

List<PeriodicThread> PeriodicThread::createdThreads;


struct periodic_struct
{
    periodic_struct(PeriodicThread *_periodic_thread, time_t _period, char* _message) {
        periodic_thread = _periodic_thread;
        period = _period;
        message = _message;
    }
    PeriodicThread *periodic_thread;
    time_t period;
    char* message;
};


void PeriodicThread::wrapper(void *arg)
{
    periodic_struct *ps = (periodic_struct*)arg;
    while (!ps->periodic_thread->terminated) {
        time_sleep(ps->period);
        ps->periodic_thread->periodicActivation((void*)ps->message);
    }
    printString("Periodic thread terminated!\n");
    delete ps;
    thread_exit();

}

void PeriodicThread::periodicActivation(void* arg) {
    char *message = (char*) arg;
    printString(message);

    int id = getId();

    printInt(id);
    printString("\n");
}

void PeriodicThread::terminate()
{
    this->terminated = true;
}

PeriodicThread::PeriodicThread(time_t period, char* message) :
        Thread(PeriodicThread::wrapper, new periodic_struct(this, period, message)) {
    this->period = period;
    this->terminated = false;
    createdThreads.addLast(this);
}