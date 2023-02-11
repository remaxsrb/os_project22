//
// Created by remax on 1/5/23.
//

#include "../h/Buffer.hpp"

buffer::buffer() : readCursor(0), writeCursor(0), size(0)
{
    _sem::createSemaphore(&mutex, 1);
    _sem::createSemaphore(&space_available, BUFFER_SIZE);
    _sem::createSemaphore(&item_available, 0);
}

buffer::~buffer()
{
    delete mutex;
    delete space_available;
    delete item_available;
}


//write kursor je na mestu za upis u bafer

//kupi sa konzole/puni bafer

void buffer::append(char c)
{
    space_available->wait();
    mutex->wait();

    //kriticka sekcija

    data[writeCursor] = c;
    this->size++;
    writeCursor = (writeCursor+1) % BUFFER_SIZE;

    mutex->signal();
    item_available->signal();
}

//salje na konzolu/prazni bafer
//read cursor je na karakteru za citanje iz bafera
char buffer::take()
{
    item_available->wait();
    mutex->wait();
    //kriticka sekcija
    char c = data[readCursor];
    readCursor = (readCursor +1) % BUFFER_SIZE;
    this->size--;

    mutex->signal();
    space_available->signal();

    return  c;
}


/*
 * Na resenju roka iz juna 2016 postoje promenjive BLOCK_SIZE i NUM_OF_BLOCKS
 * jer se u tom resenju metoda get implementirala tako sto se kupi ceo blok podataka.
 *
 * U mom slucaju posto kupim karakter po karakter, NUM_OF_BLOCKS je isto sto i BLOCK_SIZE jer jedan karakter
 * se smesta u jedan element ovog niza. Zakljucak je da je onda meni BLOCK_SIZE = 1
 * */