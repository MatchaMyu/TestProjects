#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "multiboot.h"

void paging_init(multiboot_info_t* mbi);

#endif
