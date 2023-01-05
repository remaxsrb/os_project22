//
// Created by remax on 12/22/22.
//

#include "../h/SleepingThreads.hpp"

void SleepingThreads::put(thread_t thread, time_t timeout)
{

    if(thread!= nullptr)
        thread->setThreadStatus(SLEEPING);

    if(!head)
    {
        head = new Elem(thread, nullptr, timeout);
        tail = head;
    }
    else
    {
        Elem* temp = head;
        Elem* prev = nullptr;

        while (temp != tail && timeout >= temp->relative_time) {

            /*relativno vreme za novu nit se dobija oduzimanjem vremena uspavljivanja nove niti i
             * relativnog vremena svih niti iza koje treba da se umetne u listu. To sve pod uslovom
             * da se nova nit ubacuje iza neke niti.*/

            timeout -= temp->relative_time;
            prev = temp;
            temp = temp->next;
        }

        if(temp==tail)
        {
            /*proverava se ovaj uslov jer nije nuzno timeout veci od relativnog vremena niti na kraju liste
             * iako se to proveravalo u prethodnoj petlji*/
            if(timeout >= tail->relative_time)
            {
                timeout-=tail->relative_time;
                Elem *novi = new Elem(thread, nullptr, timeout);
                tail->next = novi;
                tail = novi;
            }
            else
            {
                //situacija kada se nit umece izmedju taila i prethodnika taila
                Elem* novi = new Elem(thread, tail, timeout);

                if(prev)
                    prev->next=novi;

                    //situacija kada su head i tail isti element i ubacuje se ispred heda.
                else
                    head = novi;
            }
            tail->relative_time-=timeout;
        }
        else
        {
            Elem *novi = new Elem(thread, temp, timeout);
            if(prev)
                prev->next = novi;
            else
                head=novi;
        }
        temp->relative_time -=timeout;
    }
}

void SleepingThreads::removeAwakenedThreads()
{
    while (head!= nullptr && head->relative_time==0)
    {
        Elem *temp = head;
        head = head->next;

        if(!head)
            tail = nullptr;

        temp->data->setThreadStatus(READY);
        Scheduler::put(temp->data);
        delete temp;

    }
}

void SleepingThreads::tickFirst()
{
    if (head)
        head->relative_time -= 1;

}

time_t SleepingThreads::peekFirstSlice()
{
    if (!head)
        return -1;

    return head->relative_time;
}