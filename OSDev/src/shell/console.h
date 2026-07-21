#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

typedef enum {
    CONSOLE_MODE_VGA,
    CONSOLE_MODE_GFX
} console_mode_t;

void console_set_mode(console_mode_t mode);

void console_write(const char* text);
void console_print_hex32(uint32_t value);
void console_putc(char c);
void console_print_uint(uint32_t text);
void console_clear(void);
console_mode_t console_get_mode(void);

#endif
