//
// Created by remax on 11/30/22.
//

#ifndef OS_PROJECT2022_THREADCODES_HPP
#define OS_PROJECT2022_THREADCODES_HPP
#include "../lib/hw.h"

enum ThreadCodes : uint8 {

        CREATED = 0,
        RUNNING = 1,
        READY = 2,
        WAITING = 3,
        SLEEPING = 4,
        FINISHED = 5,
        IDLE = 6,
        };

#endif //OS_PROJECT2022_THREADCODES_HPP
