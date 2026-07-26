//Purpose: Main kernel logic entry point from kernel_main (links to Boot.s)
//Contains minor error printing and handling.

#include <stdint.h>
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "vga.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "shell/shell.h"
#include "multiboot.h"
#include "memory.h"
#include "heap.h"
#include "paging.h"
#include "string.h"
#include "graphics.h"
#include "serial.h"
#include "gfx_shell.h"
#include "shell/console.h"

static void dump_idtr();
static void draw_boot_screen(void);

static inline void trigger_bp(void) {
    __asm__ __volatile__("int3");
}

void print_multiboot_memory_map(multiboot_info_t* mbi); //Logic in memory.c

void kernel_main(uint32_t magic, multiboot_info_t* mbi) { //Probably change magic

if (magic != 0x2BADB002) {
    serial_write("BAD MULTIBOOT MAGIC\n");
}

    serial_init();

    serial_write("Kernel reached\n");

    serial_write("magic: ");
    serial_write_hex32(magic);
    serial_write("\n");

    serial_write("flags: ");
    serial_write_hex32(mbi->flags);
    serial_write("\n");

    serial_write("fb type: ");
    serial_write_hex32(mbi->framebuffer_type);
    serial_write("\n");

    serial_write("fb bpp: ");
    serial_write_hex32(mbi->framebuffer_bpp);
    serial_write("\n");

    serial_write("fb width: ");
    serial_write_hex32(mbi->framebuffer_width);
    serial_write("\n");

    serial_write("fb height: ");
    serial_write_hex32(mbi->framebuffer_height);
    serial_write("\n");

    serial_write("fb pitch: ");
    serial_write_hex32(mbi->framebuffer_pitch);
    serial_write("\n");

    uint64_t address = mbi->framebuffer_addr;

    serial_write("fb addr high: ");
    serial_write_hex32((uint32_t)(address >> 32));
    serial_write("\n");

    serial_write("fb addr low: ");
    serial_write_hex32((uint32_t)mbi->framebuffer_addr);
    serial_write("\n");

/*
//DEBUGGING: Enable this when you want to enable debugging and force GFX/Console Writes.
int graphics_enabled = gfx_init(mbi); //Graphics checker in gfx
    gfx_shell_init();
    console_set_mode(CONSOLE_MODE_GFX);
*/

    console_write("Booting EquineOS...\n");

    idt_init();
    console_write("[OK] IDT\n");
    dump_idtr();

    idt_install_exceptions();
    console_write("[OK] Install Exceptions\n");

    irq_install();
    console_write("[OK] IRQ Install\n");

    keyboard_init();

    __asm__ volatile ("sti");
    console_write("After STI Violatile\n");

    __asm__ volatile ("int3");
    console_write("After INT3 Volatile\n");
    vga_print("\n");

    print_multiboot_memory_map(mbi);

vga_print("page dir: ");
vga_print_hex32_cursor(debug_page_directory);
vga_print("\n");

vga_print("fb table: ");
vga_print_hex32_cursor(debug_fb_page_table);
vga_print("\n");

    //console_write("Trying Paging\n");
    paging_init(mbi);
    //console_write("Paging Successful\n");

//console_write("Framebuffer write survived\n");

    console_write("If this prints, you survived the boot load process!\n");

if (mbi->flags & MULTIBOOT_INFO_FRAMEBUFFER) {
    
    /*console_write("FB type: ");
    console_print_hex32(mbi->framebuffer_type);
    console_write("\n");

    console_write("FB bpp: ");
    console_print_hex32(mbi->framebuffer_bpp);
    console_write("\n");

    console_write("FB width: ");
    console_print_hex32(mbi->framebuffer_width);
    console_write("\n");

    console_write("FB height: ");
    console_print_hex32(mbi->framebuffer_height);
    console_write("\n");

    console_write("FB addr low: ");
    console_print_hex32((uint32_t)mbi->framebuffer_addr);
    console_write("\n"); */
}

    //console_write("!!!Starting Timer\n");
    timer_init(100);
    //console_write("Timer Started!!!\n");

console_write("PIC1: ");
console_print_hex32(pic_get_pic1_mask());

console_write("\nPIC2: ");
console_print_hex32(pic_get_pic2_mask());

int graphics_enabled = gfx_init(mbi); //Graphics checker in gfx
if (graphics_enabled) {

    gfx_shell_init();

    console_set_mode(CONSOLE_MODE_GFX);
} else {
    console_set_mode(CONSOLE_MODE_VGA);

    vga_clear(0x0F);

    draw_boot_screen();
    draw_uptime();
    shell_run();
    
}
    // halt forever
    while (1)
    {
        gfx_shell_process_pending();

        __asm__ volatile ("hlt");
    }

}

struct __attribute__((packed)) idtr32 {
    uint16_t limit;
    uint32_t base;
};

static void dump_idtr() {
    struct idtr32 idtr;
    __asm__ __volatile__("sidt %0" : "=m"(idtr));

    vga_print("IDTR base:");
    vga_print_hex32_cursor(idtr.base);

    vga_print("IDTR limit:");
    vga_print_hex32_cursor((uint32_t)idtr.limit);
    vga_print("\n");
}



void isr3_handler_c(void) {
    vga_print_at("ISR3: int3 reached!", 0x0A, 0, 10);
}

void draw_boot_screen(void) {
    vga_print_at("================================", 0x0F, 0, 1);
    vga_print_at("          EquineOS v0.01           ", 0x0D, 0, 2);
    vga_print_at("================================", 0x0F, 0, 3);

    vga_print_at("Boot complete.", 0x0F, 0, 5);
    vga_print_at("Interrupts: enabled", 0x0F, 0, 6);
    vga_print_at("Ready for keyboard input.", 0x0F, 0, 7);

}
