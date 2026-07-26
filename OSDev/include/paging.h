#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "multiboot.h"

void paging_init(multiboot_info_t* mbi);
extern uint32_t debug_page_directory;
extern uint32_t debug_fb_page_table;

#endif
