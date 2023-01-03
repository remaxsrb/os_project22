//
// Created by remax on 10/13/22.
//

#include "../lib/mem.h"
#include "../h/syscall_cpp.hpp"
#include "../h/MemoryAllocator.hpp"

void *__mem_alloc(size_t size)
{
    void * ret = MemoryAllocator::alloc(size);
    return ret;
}

int __mem_free(void *ptr)
{
    return MemoryAllocator::mem_free(ptr);
}

void *operator new(size_t n) noexcept
{
    return mem_alloc(n);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    mem_free(p);
}

void operator delete[](void *p) noexcept
{
    mem_free(p);
}