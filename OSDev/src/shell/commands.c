#include "commands.h"
#include "vga.h"
#include "shell.h"
#include "isr.h"
#include "memory.h"
#include "heap.h"
#include "console.h"
#include "graphics.h"
#include "app.h"
#include "apps/baimage/badapple.h"
#include "apps/doomgeneric/doomheaders/doomgeneric.h"
#include "timer.h"
#include <string.h>

//IMPLEMENT IDEAS: turn the massive long table of if/then/else into a command table.

extern int shell_row;

//Command list
void cmd_version(void) {
    console_write("EquineOS v0.01\n");
}

void cmd_clear(void) {
    console_clear();
}

void cmd_echo(char *args) {
    console_write(args);
    console_write("\n");
}

//Heap Test Command
static void cmd_heaptest(void) {
    console_write("\n");
    console_write("=== HEAP TEST START ===\n");

    void* a = kmalloc(16);
    void* b = kmalloc(32);
    void* c = kmalloc(64);
    void* d = kmalloc(16);
    void* e = 0;

    char buf[11];

    if (!a || !b || !c) {
        console_write("Heap allocation failed.\n");
        console_write("=== HEAP TEST FAILED ===\n");
        return;
    }

    console_write("Allocated blocks:\n");


    console_write("  a = ");
    console_print_hex32((uint32_t)a);
    console_write("\n");

    console_write("  b = ");
    console_print_hex32((uint32_t)b);
    console_write("\n");

    console_write("  c = ");
    console_print_hex32((uint32_t)c);
    console_write("\n");

    console_write("  d = ");
    console_print_hex32((uint32_t)d);
    console_write("\n");

    uint8_t* pa = (uint8_t*)a;
    uint8_t* pb = (uint8_t*)b;
    uint8_t* pc = (uint8_t*)c;

    for (int i = 0; i < 16; i++) pa[i] = 0xAA;
    for (int i = 0; i < 32; i++) pb[i] = 0xBB;
    for (int i = 0; i < 64; i++) pc[i] = 0xCC;

    int ok = 1;

    for (int i = 0; i < 16; i++) {
        if (pa[i] != 0xAA) ok = 0;
    }
    for (int i = 0; i < 32; i++) {
        if (pb[i] != 0xBB) ok = 0;
    }
    for (int i = 0; i < 64; i++) {
        if (pc[i] != 0xCC) ok = 0;
    }

    if (ok) {
        console_write("Pattern verification passed.\n");
    } else {
        console_write("Pattern verification FAILED.\n");
    }

    kfree(b);
    console_write("Freed block B.\n");

    e = kmalloc(24);

    console_write("Allocated e = ");
    console_print_hex32((uint32_t)e);
    console_write("\n");

    uint8_t* pe = (uint8_t*)e;
    for (int i = 0; i < 24; i++) {
    pe[i] = 0xDD;
		}

    int a_ok = 1;
    for (int i = 0; i < 16; i++) {
    if (pa[i] != 0xAA) a_ok = 0;
	}

    int c_ok = 1;
    for (int i = 0; i < 64; i++) {
    if (pc[i] != 0xCC) c_ok = 0;
	}

    if (a_ok && c_ok) {
    	console_write("Neighbor integrity passed.\n");
    } else {
    	console_write("Neighbor integrity FAILED.\n");
        if (!a_ok) console_write("  A corrupted\n");
        if (!c_ok) console_write("  C corrupted\n");
	}

    console_write("=== HEAP TEST END ===\n");
}

//Process command list
void process_command(char *input) {
    char *args = input;

    while (*args && *args != ' ') {
        args++;
    }

    if (*args == ' ') {
        *args = '\0';
        args++;
    }

    while (*args == ' ') {
        args++;
    }

    if (strcmp(input, "version") == 0) {
        cmd_version();
    } 

    else if (strcmp(input, "echo") == 0) {
        cmd_echo(args);
    }

    else if (strcmp(input, "clear") == 0) {
        cmd_clear();
    } 

    else if (strcmp(input, "heap") == 0) {
    cmd_heaptest();
    return; 
    }

    else if (strcmp(input, "app") == 0) {
        console_write("Starting app...\n");
        cmd_clear();
        //doom_test();

        console_write("App exited cleanly.\n");
        return;
    }
    else if (strcmp(input, "doom") == 0) {
        console_write("Starting Doom...\n");
        cmd_clear();
        shell_set_active(0);
        doomgeneric_Create(0, NULL);

        while (1)
        {
            doomgeneric_Tick();
        }

        shell_set_active(1);

        console_write("Doom exited.\n");
        return;
    }
    else if (strcmp(input, "badapple") == 0) {
    badapple_main();
    return;
    }
    else if (strcmp(input, "sleep") == 0) {
    sleep_test();
    return;
    }

    else if (strcmp(input, "mem") == 0) {
    mem_stats_t stats;
    char buf[32];

    mem_get_stats(&stats);

    console_write("Heap stats:\n");

    console_write("Used bytes: ");
    console_print_uint(stats.used_bytes);
    console_write("\n");

    console_write("Free bytes: ");
    console_print_uint(stats.free_bytes);
    console_write("\n");

    console_write("Blocks: ");
    console_print_uint(stats.block_count);
    console_write("\n");

    console_write("Free blocks: ");
    console_print_uint(stats.free_block_count);
    console_write("\n");

    console_write("Largest free block: ");
    console_print_uint(stats.largest_free_block);
    console_write("\n");

    } else if (strcmp(input, "heap") == 0) {
    char buf[32];
    uint32_t start = heap_get_start();
    uint32_t end   = heap_get_end();
    uint32_t limit = heap_get_limit();

    console_write("Heap info:\n");

    console_write("Start: ");
    console_print_hex32(start);
    console_write("\n");

    console_write("End: ");
    console_print_hex32(end);
    console_write("\n");

    console_write("Limit: ");
    console_print_hex32(limit);
    console_write("\n");

    console_write("Consumed bytes: ");
    console_print_uint(end - start);
    console_write("\n");

    console_write("Remaining bytes: ");
    console_print_uint(limit - end);
    console_write("\n");

    return;
    }//Panic command
     else if (strcmp(input, "panic") == 0) {
	panic("Manual Pony Prompted Panic", 0);
	return;
	}
    //Invalid command.
    else {
        console_write("Neigh, that is not a valid command.\n");
    }

}
//end process command list.
