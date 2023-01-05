//
// Created by remax on 1/5/23.
//

#include "../h/Buffer.hpp"

buffer::buffer() : readCursor(0), writeCursor(0), size(0)
{
    _sem::createSemaphore(&mutex, 1);
    _sem::createSemaphore(&space_availavle, 0);
    _sem::createSemaphore(&item_available, 0);
}

buffer::~buffer()
{
    delete mutex;
    delete space_availavle;
    delete item_available;
}

//read cursor je na karakteru za citanje iz bafera
//write kursor je na mestu za upis u bafer

//pise na konzolu

void buffer::put_char(char c)
{
    space_availavle->wait();
    mutex->wait();

    data[writeCursor] = c;
    writeCursor = (writeCursor+1) % BUFFER_SIZE;

    bool signalItemAvailable = writeCursor != (BUFFER_SIZE-1);

    mutex->signal();
    if(signalItemAvailable)
        item_available->signal();
}

//cita sa konzole
char buffer::get_char()
{
    item_available->wait();
    mutex->wait();

    char c = data[readCursor];
    readCursor = (readCursor +1) % BUFFER_SIZE;
    this->size--;
    mutex->signal();
    space_availavle->signal();

    return  c;
}