#include "heap.h"
#include "isr.h"
#include <string.h>
#include <stdint.h>

#include "shell/console.h"

#define MIN_SPLIT_SIZE 8 //Defines minimum split size
#define HEAP_CANARY 0xDEADBEEF

//Block header
typedef struct block_header {
    uint32_t magic;
    uint32_t size;
    uint32_t requested_size;
    uint32_t free;
    struct block_header* next;
} block_header_t;
#define ALLOC_MAGIC 0xC0FFEE42 

static block_header_t* heap_head = 0;

static uint32_t heap_start;
static uint32_t heap_end;
static uint32_t heap_limit;

void heap_init(uint32_t start, uint32_t limit) {
    heap_start = start;
    heap_end = start;
    heap_limit = limit;
    heap_head = 0;
}

uint32_t heap_get_start(void) {
    return heap_start;
}

uint32_t heap_get_end(void) {
    return heap_end;
}

uint32_t heap_get_limit(void) {
    return heap_limit;
}

//split block
static void split_block(block_header_t* block, uint32_t size) {
    if (block->size < size + sizeof(block_header_t) + MIN_SPLIT_SIZE) {
        return;
    }

    uint32_t new_block_addr =
        (uint32_t)block + sizeof(block_header_t) + size;

    block_header_t* new_block = (block_header_t*)new_block_addr;
    new_block->magic = ALLOC_MAGIC;
    new_block->size = block->size - size - sizeof(block_header_t);
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
}

//Create block
static block_header_t* create_block(uint32_t size) {
    uint32_t total_size = sizeof(block_header_t) + size;

    if (heap_end >= heap_limit) return 0;
    if (total_size > heap_limit - heap_end) return 0;

    block_header_t* block = (block_header_t*)heap_end;

    block->magic = ALLOC_MAGIC;
    block->size = size;
    block->requested_size = 0;
    block->free = 0;
    block->next = 0;

    heap_end += total_size;
    return block;
}

//Free Block
static block_header_t* find_free_block(uint32_t size) {
    block_header_t* current = heap_head;

    while (current) {
        if (current->magic == ALLOC_MAGIC &&
            current->free &&
            current->size >= size) {
            return current;
        }
        current = current->next;
    }

    return 0;
}

//colesce blocks
static void coalesce_free_blocks(void) {
    block_header_t* current = heap_head;

    while (current && current->next) {
        uint32_t current_end =
            (uint32_t)current + sizeof(block_header_t) + current->size;

        if (current->magic == ALLOC_MAGIC &&
            current->next->magic == ALLOC_MAGIC &&
            current->free &&
            current->next->free &&
            current_end == (uint32_t)current->next) {

            block_header_t* next = current->next;

            current->size += sizeof(block_header_t) + next->size;
            current->next = next->next;

        } else {
            current = current->next;
        }
    }
}

//kmalloc
void* kmalloc(size_t size) {
    if (size == 0) return 0;

    if (size > 0xFFFFFFFF - sizeof(uint32_t) - 7) return 0;

    uint32_t requested_size = (uint32_t)size;
    uint32_t aligned_size = (requested_size + sizeof(uint32_t) + 7) & ~7;

    block_header_t* block = find_free_block(aligned_size);

    if (block) {
        split_block(block, aligned_size);
        block->free = 0;
        block->requested_size = requested_size;

        void* user_ptr = (void*)((uint32_t)block + sizeof(block_header_t));
        uint32_t canary_offset = (requested_size + 3) & ~3;
        uint32_t* canary = (uint32_t*)((uint32_t)user_ptr + canary_offset);
        *canary = HEAP_CANARY;

        return user_ptr;
    }

    block = create_block(aligned_size);

    if (!block) {
        return 0;
    }

    block->requested_size = requested_size;

    if (!heap_head) {
        heap_head = block;
    } else {
        block_header_t* current = heap_head;
        while (current->next) {
            current = current->next;
        }
        current->next = block;
    }

    void* user_ptr = (void*)((uint32_t)block + sizeof(block_header_t));
    uint32_t canary_offset = (requested_size + 3) & ~3;
    uint32_t* canary = (uint32_t*)((uint32_t)user_ptr + canary_offset);
    *canary = HEAP_CANARY;

    return user_ptr;

}
//kfree
void kfree(void* ptr) {
    if (!ptr) return;

    block_header_t* block =
        (block_header_t*)((uint32_t)ptr - sizeof(block_header_t));

    uint32_t canary_offset =
        (block->requested_size + 3) & ~3;

    uint32_t* canary =
        (uint32_t*)((uint32_t)ptr + canary_offset);

    if (*canary != HEAP_CANARY) {
        panic("Heap overflow detected", 0);
    }

    if (block->magic != ALLOC_MAGIC) {
        panic("Invalid free detected", 0);
    }

    if (block->free) {
        panic("Double free detected", 0);
    }

    block->free = 1;
    coalesce_free_blocks();
}

//for the MEM command used in commands.c
void mem_get_stats(mem_stats_t* stats) {
    if (!stats) {
        return;
    }

    stats->used_bytes = 0;
    stats->free_bytes = 0;
    stats->block_count = 0;
    stats->free_block_count = 0;
    stats->largest_free_block = 0;

    block_header_t* current = heap_head;

    while (current) {
        if (current->magic != ALLOC_MAGIC) {
            return;
        }

        stats->block_count++;

        if (current->free) {
            stats->free_bytes += current->size;
            stats->free_block_count++;

            if (current->size > stats->largest_free_block) {
                stats->largest_free_block = current->size;
            }
        } else {
            stats->used_bytes += current->size;
        }

        current = current->next;
    }
}

void* malloc(size_t size)
{
    return kmalloc((uint32_t)size);
}

void free(void* ptr)
{
    kfree(ptr);
}

void* realloc(void* ptr, size_t new_size)
{
    if (ptr == NULL)
        return malloc(new_size);

    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    block_header_t* old_block =
        (block_header_t*)((uint32_t)ptr - sizeof(block_header_t));

    if (old_block->magic != ALLOC_MAGIC || old_block->free) {
        panic("Invalid realloc detected", 0);
    }

    uint32_t old_size = old_block->requested_size;

    void* new_ptr = malloc(new_size);
    if (!new_ptr)
        return NULL;

    uint32_t copy_size = old_size < new_size ? old_size : new_size;
    memcpy(new_ptr, ptr, copy_size);

    free(ptr);

    return new_ptr;
}

void* calloc(size_t count, size_t size)
{
    if (count == 0 || size == 0)
        return NULL;

    // Optional overflow check
    if (count > SIZE_MAX / size)
        return NULL;

    size_t total = count * size;

    void* ptr = malloc(total);
    if (!ptr)
        return NULL;

    memset(ptr, 0, total);

    return ptr;
}
