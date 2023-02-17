//
// Created by remax on 10/13/22.
//

#ifndef OS_PROJECT2022_MEMORYPIECE_HPP
#define OS_PROJECT2022_MEMORYPIECE_HPP

#include "../lib/hw.h"

enum MemoryPieceStatus {
    FREE,
    ALLOCATED
};

typedef struct MemoryPiece {
    size_t size;
    struct MemoryPiece *next;
    struct MemoryPiece *prev;
    enum MemoryPieceStatus status;

} MemoryPiece;

void insert(MemoryPiece **head_ptr, MemoryPiece *node, enum MemoryPieceStatus status);

void remove(MemoryPiece **head_ptr, MemoryPiece *node);

void merge_blocks(MemoryPiece *free); //K2 2015 zad2


#endif //OS_PROJECT2022_MEMORYPIECE_HPP
