//
// Created by remax on 10/13/22.
//

#ifndef OS_PROJECT2022_MEMORYALLOCATOR_HPP
#define OS_PROJECT2022_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"
#include "MemoryPiece.hpp"

class MemoryAllocator {
public:
    static MemoryPiece *free, *allocated;

    static uint64 allocd;

    static uint64 deallocd;

    static void *alloc(size_t);

    static int mem_free(void*);

    static void initialise_memory();

};


#endif //OS_PROJECT2022_MEMORYALLOCATOR_HPP
