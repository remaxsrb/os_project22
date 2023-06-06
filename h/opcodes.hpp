//
// Created by remax on 10/13/22.
//

#ifndef OS_PROJECT2022_OPCODES_HPP
#define OS_PROJECT2022_OPCODES_HPP
#include "../lib/hw.h"

enum Opcodes: uint64 {
    MEM_ALLOC       = 0x01,
    MEM_FREE        = 0x02,
    THREAD_CREATE   = 0x11,
    THREAD_EXIT     = 0x12,
    THREAD_DISPATCH = 0x13,
    THREAD_JOIN = 0x14,
    THREAD_START    = 0x15,
    THREAD_DELETE   = 0x16,
    THREAD_PREPARE   = 0x17,
    SEM_OPEN        = 0x21,
    SEM_CLOSE       = 0x22,
    SEM_WAIT        = 0x23,
    SEM_SIGNAL      = 0x24,
    TIME_SLEEP      = 0x31,
    GET_C            = 0x41,
    PUT_C            = 0x42,
};


#endif //OS_PROJECT2022_OPCODES_HPP
