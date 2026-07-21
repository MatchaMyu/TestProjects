#include <stdint.h>
#include "paging.h"
#include "heap.h"
#include "vga.h"
#include "multiboot.h"

#define PAGE_SIZE 4096
#define PAGE_PRESENT 0x1
#define PAGE_RW      0x2
#define IDENTITY_TABLES 64
#define PAGE_TABLE_ENTRIES 1024

#define ALIGN_UP(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

extern void load_page_directory(uint32_t* page_directory);
extern void enable_paging(void);

void paging_init(multiboot_info_t* mbi) {
    uint32_t raw = (uint32_t)kmalloc(4096 + IDENTITY_TABLES * 4096 + 4096);
    uint32_t aligned = ALIGN_UP(raw, 4096);

    uint32_t* page_directory = (uint32_t*)aligned;
    uint32_t* identity_tables = (uint32_t*)(aligned + 4096);

    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
    }

    for (int table = 0; table < IDENTITY_TABLES; table++) {
        uint32_t* table_ptr = identity_tables + table * 1024;

        for (int i = 0; i < 1024; i++) {
            uint32_t addr = (table * 1024 + i) * PAGE_SIZE;
            table_ptr[i] = addr | PAGE_PRESENT | PAGE_RW;
        }

        page_directory[table] =
            ((uint32_t)table_ptr) | PAGE_PRESENT | PAGE_RW;
    }

    uint32_t* fb_page_table =
        (uint32_t*)ALIGN_UP((uint32_t)kmalloc(4096 + 4096), 4096);

    for (int i = 0; i < 1024; i++) {
        fb_page_table[i] = 0;
    }

    uint32_t fb_base = (uint32_t)mbi->framebuffer_addr;
    uint32_t fb_dir_index = fb_base >> 22;
    uint32_t fb_table_base = fb_base & 0xFFC00000;

    for (int i = 0; i < 1024; i++) {
        fb_page_table[i] =
            (fb_table_base + i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
    }

    page_directory[fb_dir_index] =
        ((uint32_t)fb_page_table) | PAGE_PRESENT | PAGE_RW;

    load_page_directory(page_directory);
    enable_paging();

    vga_print("Paging enabled!\n");
}
