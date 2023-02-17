//
// Created by remax on 10/13/22.
//

#ifndef OS_PROJECT2022_MEMORYALLOCATOR_HPP
#define OS_PROJECT2022_MEMORYALLOCATOR_HPP

//resenje kombinuje razne rokove
//

#include "../lib/hw.h"
#include "MemoryPiece.hpp"

class MemoryAllocator {
public:
    static MemoryPiece *free, *allocated;

    static uint64 allocd;

    static uint64 deallocd;

    static void *alloc(size_t); //k2 april 2014 zad2

    static int mem_free(void*); // k2 2015 septembar zad2

    static void initialise_memory();

};


#endif //OS_PROJECT2022_MEMORYALLOCATOR_HPP
