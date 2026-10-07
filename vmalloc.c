#include "vm.h"
#include "vmlib.h"

/**
 * Allocate a memory block with the given minimum size on the simulated heap.
 * If allocation succeeds, return a pointer to the start of the new block's
 * payload. If allocation fails due to insufficient free space, return NULL.
 *
 * Read the section for vmalloc in the PA writeup for how to implement this
 * function.
 */
void *vmalloc(size_t size) {
    // TODO
    if (size <= 0) {
        return NULL;
    }
    size += 8;
    if (size % 16 != 0) {
        size = (size - (size % 16)) + 16;
    }
    struct block_header *current = heapstart;
    size_t biggest = 0;
    struct block_header *best_fit = NULL;
    struct block_header *prevBlock = NULL;
    struct block_header *prevBlockBest = NULL;
    while (1) {

        size_t block_size = BLKSZ(current);
        if (block_size == 0) {
            break;
        }

        if (!(current->size_status & VM_BUSY) && block_size == size) {
            current->size_status = size;
            if (prevBlock != NULL) {
                if (prevBlock->size_status & VM_BUSY) {
                    current->size_status |= VM_PREVBUSY;
                }
            }else{
                current->size_status |= VM_PREVBUSY;
            }
            current->size_status |= VM_BUSY;
            struct block_header *next_block =
                (struct block_header *)((char *)current + size);
            if (!(BLKSZ(next_block) == 0)) {
                next_block->size_status |= VM_PREVBUSY;
            }
            return (char *)current + sizeof(struct block_header);
        } else if (!(current->size_status & VM_BUSY) && block_size > size) {
            if (best_fit == NULL) {
                best_fit = current;
                biggest = BLKSZ(current);
            } else {
                if (block_size < biggest) {
                    biggest = block_size;
                    prevBlockBest = prevBlock;
                    best_fit = current;
                }
            }
        }
        prevBlock = current;
        current = (struct block_header *)((char *)current + block_size);
    }
    if (best_fit != NULL) {
        struct block_header *free_block =
            (struct block_header *)((char *)best_fit + size);
        free_block->size_status = biggest - size;
        free_block->size_status &= ~VM_BUSY;
        free_block->size_status |= VM_PREVBUSY;

        struct block_footer *free_block_footer =
            (struct block_footer *)((char *)free_block + (biggest - size) -
                                    sizeof(struct block_footer));
        free_block_footer->size = biggest - size;


        best_fit->size_status = size;
        if (prevBlockBest != NULL) {
            if (prevBlockBest->size_status & VM_BUSY) {
                best_fit->size_status |= VM_PREVBUSY;
            }
        }else{
            best_fit->size_status |= VM_PREVBUSY;
        }
    
        
        best_fit->size_status |= VM_BUSY;
        return ((char *)best_fit + sizeof(struct block_header));
    }
    return NULL;
}
