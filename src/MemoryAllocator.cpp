//
// Created by remax on 10/13/22.
//

#include "../h/MemoryAllocator.hpp"
#include "../tests/printing.hpp"

MemoryPiece *MemoryAllocator::free = nullptr;

MemoryPiece *MemoryAllocator::allocated = nullptr;

uint64 MemoryAllocator::allocd = 0;

uint64 MemoryAllocator::deallocd = 0;

void MemoryAllocator::initialise_memory()
{
    free = (MemoryPiece*)HEAP_START_ADDR;

    MemoryPiece  *mem = free;

    mem->prev=mem->next= nullptr;
    mem->size = (size_t)((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - MEM_BLOCK_SIZE);
    mem->status=FREE;
}

void *MemoryAllocator::alloc(size_t requested_size)
{
    //ako broj blokova nije deljiv sa velicinom bloka uradi peding
    if(requested_size % MEM_BLOCK_SIZE != 0)
        requested_size = (requested_size / MEM_BLOCK_SIZE + 1 ) * MEM_BLOCK_SIZE;


    //ako je alokacija uspesna vraca se adresa pocetnog bloka

    for (MemoryPiece* current = free; current; current=current->next)
    {
        if (current->size==requested_size)
        {
            remove(&free, current);
            insert(&allocated, current, ALLOCATED);
            allocd+=requested_size;
            return (void*)((char*)current + sizeof(MemoryPiece));
        }

            /*ako je slobodni segment veci od trazene kolicine memorije onda se ovaj segment
             * razbija na dva dela, jedan novi segment ce biti alociran i trazene velicine dok ce visak
             * koji ostane u drugom segmentu biti vracen u listu slobodnih segmenata*/

        else if (current->size > requested_size)
        {

            remove(&free, current);

            //Ova nova adresa je ona koja se dobija kada bi originalni segment presekli na izracunatom mestu
            MemoryPiece* new_node = (MemoryPiece*)((char*)current + sizeof(MemoryPiece) + requested_size);

            new_node->prev=new_node->next= nullptr;
            new_node->size = (current->size-sizeof(MemoryPiece)-requested_size);

            current->size = requested_size;

            insert(&free, new_node, FREE);
            insert(&allocated, current, ALLOCATED);

            char* returnval = (char*)current+sizeof(MemoryPiece);
            allocd+=requested_size;

            return (void*)(returnval);

        }
    }
    return nullptr; //nema odgovarajuceg segmenta u memoriji

}

int MemoryAllocator::mem_free(void *mem_to_free)
{
    if(!mem_to_free)
        return -1;

    MemoryPiece * mem_desc = (MemoryPiece*)((char*)mem_to_free-sizeof(MemoryPiece));

    if(mem_desc->status != ALLOCATED)
        return -1;

    deallocd+=mem_desc->size;

    remove(&allocated, mem_desc);
    insert(&free, mem_desc, FREE);
    //pozivanje metode za spajanje blokova nakon svakog oslobadjanja memorije kako bi se izbegla fragmentacija
    merge_blocks(mem_desc);

    return 1;

}

size_t MemoryAllocator::mem_get_free_space() {

    size_t free_space = 0;

    for (MemoryPiece* current = free; current; current=current->next) {
        free_space+=current->size;
    }
    return free_space;
}

size_t MemoryAllocator::mem_get_largest_free_block() {
    size_t largest_free_block = 0;
    for (MemoryPiece* current = allocated; current; current=current->next) {
        if (current->size > largest_free_block)
            largest_free_block = current->size;
    }
    return largest_free_block;

}