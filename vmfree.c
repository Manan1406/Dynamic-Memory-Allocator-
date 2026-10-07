#include "vm.h"
#include "vmlib.h"

/**
 * The vmfree() function frees the memory space pointed to by ptr,
 * which must have been returned by a previous call to vmalloc().
 * Otherwise, or if vmfree(ptr) has already been called before,
 * undefined behavior occurs.
 *
 * If ptr is NULL, no operation is performed.
 */
void vmfree(void *ptr) {
    // TODO
    if (ptr == NULL) {
        return;
    }
    struct block_header *block_to_free =
        (struct block_header *)((char *)ptr - sizeof(struct block_header));
    if (!(block_to_free->size_status & VM_BUSY)) {
        return;
    }
    block_to_free->size_status &= ~VM_BUSY;

    struct block_footer *footer =
        (struct block_footer *)((char *)block_to_free + BLKSZ(block_to_free) -
                                sizeof(struct block_footer));
    footer->size = BLKSZ(block_to_free);
    

    struct block_header *next_block = (struct block_header *)((char *)block_to_free + BLKSZ(block_to_free));
    next_block->size_status &= ~VM_PREVBUSY;
    if (BLKSZ(next_block) != 0 && !(next_block->size_status & VM_BUSY)){
        size_t next_block_size = BLKSZ(next_block);
        int check = block_to_free->size_status & VM_PREVBUSY;
        block_to_free->size_status = BLKSZ(block_to_free) + next_block_size;
        footer = (struct block_footer *)((char *)next_block + next_block_size -sizeof(struct block_footer));
        footer->size = BLKSZ(block_to_free);
        block_to_free->size_status &= ~VM_BUSY;
        if(check != 0){
            block_to_free->size_status += 2;
        }
    }

    if (!(block_to_free->size_status & VM_PREVBUSY)) {
        struct block_footer *prev_footer =
            (struct block_footer *)((char *)block_to_free -
                                    sizeof(struct block_footer));
        struct block_header *prev_header =
            (struct block_header *)((char *)prev_footer - prev_footer->size +
                                    sizeof(struct block_footer));
        prev_header->size_status = BLKSZ(prev_header) + BLKSZ(block_to_free);
        footer = (struct block_footer *)((char *)block_to_free +
                                         BLKSZ(block_to_free) -
                                         sizeof(struct block_footer));
        prev_header->size_status |= VM_PREVBUSY;
        prev_header->size_status &= ~VM_BUSY;
        footer->size = BLKSZ(prev_header);
        
    }


}
