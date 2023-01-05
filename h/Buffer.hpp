//
// Created by remax on 1/5/23.
//

#ifndef OS_PROJECT2022_BUFFER_HPP
#define OS_PROJECT2022_BUFFER_HPP

#include "_semaphore.hpp"

//implementacija bafera uzeta sa treceg kolokvijuma iz juna 2016


class buffer {

private:
    static const size_t BUFFER_SIZE = 256;
    char data[BUFFER_SIZE];
    uint64 readCursor;
    uint64 writeCursor;
    uint64 size;
    sem_t mutex;
    sem_t space_availavle;
    sem_t item_available;

public:

    buffer();
    ~buffer();

    char get_char();
    void put_char(char c);

    uint64 getSize() const {return  this->size;}

    void *operator new(size_t size) { return __mem_alloc(size); }

    void operator delete(void *ptr) { __mem_free(ptr); }


};


#endif //OS_PROJECT2022_BUFFER_HPP
