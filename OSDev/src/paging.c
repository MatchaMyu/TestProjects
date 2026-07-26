#include <stdint.h>
#include "paging.h"
#include "heap.h"
#include "vga.h"
#include "multiboot.h"
#include "shell/console.h"

#define PAGE_SIZE 4096
#define PAGE_PRESENT 0x1
#define PAGE_RW      0x2
#define IDENTITY_TABLES 64
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_PCD 0x10
#define PAGE_PWT     0x08

#define ALIGN_UP(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

extern void load_page_directory(uint32_t* page_directory);
extern void enable_paging(void);

uint32_t debug_page_directory;
uint32_t debug_fb_page_table;

void paging_init(multiboot_info_t* mbi) {
    uint32_t raw = (uint32_t)kmalloc(4096 + IDENTITY_TABLES * 4096 + 4096);
    uint32_t aligned = ALIGN_UP(raw, 4096);

/* DEBUG
vga_print("raw: ");
vga_print_hex32_cursor(raw);
vga_print("\n");

vga_print("aligned: ");
vga_print_hex32_cursor(aligned);
vga_print("\n");*/

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
    uint32_t fb_start = fb_base & ~0xFFF;
    uint32_t fb_offset = fb_base & 0xFFF;
    uint32_t fb_size = mbi->framebuffer_pitch * mbi->framebuffer_height;
    uint32_t fb_pages = (fb_offset + fb_size + PAGE_SIZE - 1) / PAGE_SIZE;

for (uint32_t i = 0; i < fb_pages; i++)
{
    fb_page_table[i] =
        (fb_start + i * PAGE_SIZE) |
        PAGE_PRESENT |
        PAGE_RW |
        PAGE_PCD |
        PAGE_PWT;
}

    page_directory[fb_dir_index] =
        ((uint32_t)fb_page_table) | PAGE_PRESENT | PAGE_RW;


console_write("PD: ");
console_print_hex32((uint32_t)page_directory);
console_write("\n");

console_write("ID tables: ");
console_print_hex32((uint32_t)identity_tables);
console_write("\n");

console_write("FB table: ");
console_print_hex32((uint32_t)fb_page_table);
console_write("\n");

    load_page_directory(page_directory);

debug_page_directory = (uint32_t)page_directory;
debug_fb_page_table = (uint32_t)fb_page_table;

    enable_paging();

    console_write("Paging enabled!\n");


uint32_t cr3;
__asm__ volatile ("mov %%cr3, %0" : "=r"(cr3));

console_write("CR3: ");
console_print_hex32(cr3);
console_write("\n");

}
