#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t used_bytes;
    uint32_t free_bytes;
    uint32_t block_count;
    uint32_t free_block_count;
    uint32_t largest_free_block;
} mem_stats_t;

void heap_init(uint32_t start, uint32_t limit);
void* kmalloc(size_t size);
void* malloc(size_t size);
void kfree(void* ptr);
void free(void* ptr);
void mem_get_stats(mem_stats_t* stats);

uint32_t heap_get_start(void);
uint32_t heap_get_end(void);
uint32_t heap_get_limit(void);

#endif
