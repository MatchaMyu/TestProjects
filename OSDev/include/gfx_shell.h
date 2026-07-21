// gfx_shell.h

#ifndef GFX_SHELL_H
#define GFX_SHELL_H
#include <stdint.h>
#include <stddef.h>

void gfx_shell_init(void);
void gfx_shell_putchar(char c);
void gfx_shell_print(const char* s);
void gfx_shell_input_char(char c);
void gfx_shell_print_uint(uint32_t value);
void gfx_shell_backspace(void);
void gfx_shell_execute(void);
void gfx_update_hw_cursor(void);
void gfx_shell_clear(uint32_t color);

void gfx_shell_print_at(const char* str, uint32_t color, int col, int row);
void gfx_putc_at(char c, uint32_t color, int x, int y);
void gfx_shell_panic_clear(uint32_t color);
void gfx_shell_print_hex32_at(uint32_t value, uint32_t color, int col, int row);
void gfx_shell_process_pending(void);

int printf(const char* fmt, ...);
#endif
