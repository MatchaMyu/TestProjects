//Purpose: Main kernel logic entry point from kernel_main (links to Boot.s)
//Contains minor error printing and handling.

#include <stdint.h>
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "vga.h"
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

    serial_write("fb addr low: ");
    serial_write_hex32((uint32_t)mbi->framebuffer_addr);
    serial_write("\n");

    vga_print("Booting EquineOS...\n");

    idt_init();
    vga_print("[OK] IDT\n");
    dump_idtr();

    idt_install_exceptions();
    vga_print("[OK] Install Exceptions\n");

    irq_install();
    vga_print("[OK] IRQ Install\n");

    __asm__ volatile ("sti");
    vga_print("After STI Violatile\n");

    __asm__ volatile ("int3");
    vga_print("After INT3 Volatile\n");
    vga_print("\n");

    print_multiboot_memory_map(mbi);

    paging_init(mbi);
    vga_print("Paging Successful\n");

    vga_print("If this prints, you survived the boot load process!\n");

if (mbi->flags & MULTIBOOT_INFO_FRAMEBUFFER) {
    
    vga_print("FB type: ");
    vga_print_hex32_cursor(mbi->framebuffer_type);
    vga_print("\n");

    vga_print("FB bpp: ");
    vga_print_hex32_cursor(mbi->framebuffer_bpp);
    vga_print("\n");

    vga_print("FB width: ");
    vga_print_hex32_cursor(mbi->framebuffer_width);
    vga_print("\n");

    vga_print("FB height: ");
    vga_print_hex32_cursor(mbi->framebuffer_height);
    vga_print("\n");

    vga_print("FB addr low: ");
    vga_print_hex32_cursor((uint32_t)mbi->framebuffer_addr);
    vga_print("\n");
}
    timer_init(100);
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
