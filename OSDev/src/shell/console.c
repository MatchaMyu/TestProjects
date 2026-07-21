#include "console.h"
#include "shell.h"
#include "gfx_shell.h"
#include "vga.h"
#include "graphics.h"

static console_mode_t current_console_mode = CONSOLE_MODE_VGA;

void console_set_mode(console_mode_t mode) {
    current_console_mode = mode;
}

console_mode_t console_get_mode(void) {
    return current_console_mode;
}

void console_write(const char* text) {
    if (current_console_mode == CONSOLE_MODE_GFX) {
        gfx_shell_print(text); //GFX Text
    } else {
        shell_print(text); //VGA Text
    }
}

void console_print_uint(uint32_t text) {
    if (current_console_mode == CONSOLE_MODE_GFX) {
        gfx_shell_print_uint(text); //GFX Text
    } else {
        vga_print_uint(text); //VGA Text
    }
}

void console_print_hex32(uint32_t value) {
    char buffer[11];
    u32_to_hex_string(value, buffer);
    console_write(buffer);
}

void console_putc(char c) {
    if (current_console_mode == CONSOLE_MODE_GFX) {
        gfx_shell_input_char(c);
    } else {
        vga_put_char(c);  
    }
}

void console_clear(void) {
    if (current_console_mode == CONSOLE_MODE_GFX) {
        gfx_shell_clear(0x000000);
    } else {
        shell_clear();
    }
}

