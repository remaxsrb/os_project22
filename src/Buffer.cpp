//
// Created by remax on 1/5/23.
//

#include "../h/Buffer.hpp"

buffer::buffer() : readCursor(0), writeCursor(0), size(0)
{
    //_sem::createSemaphore(&mutex_take, 1);
    //_sem::createSemaphore(&mutex_append, 1);

    _sem::createSemaphore(&mutex, 1);

    _sem::createSemaphore(&space_available, BUFFER_SIZE);
    _sem::createSemaphore(&item_available, 0);
}

buffer::~buffer()
{
    //delete mutex_take;
    //delete mutex_append;
    delete mutex;
    delete space_available;
    delete item_available;
}


//write kursor je na mestu za upis u bafer

//kupi sa konzole/puni bafer

void buffer::kernel_append(char c)
{
    space_available->wait();
    //mutex_append->wait();
    mutex->wait();
    data[writeCursor] = c;
    this->size++;
    writeCursor = (writeCursor+1) % BUFFER_SIZE;

    mutex->signal();
    //mutex_append->signal();
    item_available->signal();
}

//salje na konzolu/prazni bafer
//read cursor je na karakteru za citanje iz bafera
char buffer::kernel_take()
{
    item_available->wait();
    //mutex_take->wait();
    mutex->wait();

    char c = data[readCursor];
    readCursor = (readCursor +1) % BUFFER_SIZE;
    this->size--;

    mutex->signal();
    //mutex_take->signal();
    space_available->signal();

    return  c;
}

//char buffer::user_take()
//{
//    sem_wait(item_available);
//    sem_wait(mutex_take);
//
//    char c = data[readCursor];
//    readCursor = (readCursor +1) % BUFFER_SIZE;
//    this->size--;
//
//    sem_signal(item_available);
//    sem_signal(mutex_take);
//
//    return  c;
//}

/*
 * Na resenju roka iz juna 2016 postoje promenjive BLOCK_SIZE i NUM_OF_BLOCKS
 * jer se u tom resenju metoda get implementirala tako sto se kupi ceo blok podataka.
 *
 * U mom slucaju posto kupim karakter po karakter, NUM_OF_BLOCKS je isto sto i BLOCK_SIZE jer jedan karakter
 * se smesta u jedan element ovog niza. Zakljucak je da je onda meni BLOCK_SIZE = 1
 * */