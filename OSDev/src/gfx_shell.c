// gfx_shell.c
//the commands.c file takes BOTH this and VGA.
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include "gfx_shell.h"
#include "graphics.h"
#include "font8x8.h"
#include "string.h"
#include "shell/commands.h"
#include "shell/console.h"
#include "shell/shell.h"
#include "keyboard.h"
#include "vga.h"
#include "ports.h"

#define SHELL_X 16
#define SHELL_Y 16
#define SHELL_W 608
#define SHELL_H 400

#define CHAR_W 8
#define CHAR_H 8

#define MAX_INPUT 128

static uint8_t* fb = 0;
static uint32_t fb_width = 0;
static uint32_t fb_height = 0;
static uint32_t fb_pitch = 0;
static uint8_t fb_bpp = 0;

static int cursor_x = SHELL_X;
static int cursor_y = SHELL_Y;

static char input[MAX_INPUT];
static int input_len = 0;
static volatile int command_ready = 0;

static void gfx_shell_draw_rect(int x, int y, int w, int h, uint32_t color) {
    gfx_fill_rect(x, y, w, 1, color);           // top
    gfx_fill_rect(x, y + h - 1, w, 1, color);   // bottom
    gfx_fill_rect(x, y, 1, h, color);           // left
    gfx_fill_rect(x + w - 1, y, 1, h, color);   // right
}

void gfx_shell_putchar(char c) {
    if (c == '\n') {
        cursor_x = SHELL_X;
        cursor_y += CHAR_H;
        return;
    }

    if (c == '\b') {
        gfx_shell_backspace();
        return;
    }

    gfx_draw_char(cursor_x, cursor_y, c, 0xFFFFFF);
    cursor_x += CHAR_W;

    if (cursor_x >= SHELL_X + SHELL_W - CHAR_W) {
        cursor_x = SHELL_X;
        cursor_y += CHAR_H;
    }
}

//
void gfx_putc_at(char c, uint32_t color, int col, int row)
{
    int x = col * CHAR_W;
    int y = row * CHAR_H;

    gfx_draw_char(x, y, c, color);
}

void gfx_shell_print(const char* str) {
    if (!str) return;
    while (*str) {
        gfx_shell_putchar(*str++);
    }
}

void gfx_shell_print_at(const char* str, uint32_t color, int col, int row)
{
    int x = col;

    while (*str) {
        gfx_draw_char_cell(x, row, *str, color);
        x++;
        str++;
    }
}

void gfx_shell_print_hex32(uint32_t value) {
    const char* hex = "0123456789ABCDEF";

    gfx_shell_putchar('0');
    gfx_shell_putchar('x');

    for (int shift = 28; shift >= 0; shift -= 4) {
        gfx_shell_putchar(hex[(value >> shift) & 0xF]);
    }
}

void gfx_shell_print_hex32_at(uint32_t value, uint32_t color, int col, int row) {
    char buffer[11];
    u32_to_hex_string(value, buffer);

    gfx_shell_print_at(buffer, color, col, row);
}

void gfx_shell_init(void) {
    gfx_clear(0x202020);

    gfx_fill_rect(8, 8, 624, 464, 0x303030);
    gfx_shell_draw_rect(8, 8, 624, 464, 0xFFFFFF);

    cursor_x = SHELL_X;
    cursor_y = SHELL_Y;

    gfx_shell_print("EquineOS Graphical Shell\n");
    gfx_shell_print("> ");
}


void gfx_shell_input_char(char c) {

    if (!shell_is_active())
    {
        return;
    }

    if (c == '\n')
    {
        cursor_x = SHELL_X;
        cursor_y += CHAR_H;

        input[input_len] = '\0';
        command_ready = 1;

        return;
    }

    if (c == '\b') {
        gfx_shell_backspace();
        return;
    }

    if (input_len < MAX_INPUT - 1) {
        input[input_len++] = c;
        gfx_shell_putchar(c);
    }
}

void gfx_shell_process_pending(void)
{
    if (!command_ready)
    {
        return;
    }

    command_ready = 0;

    process_command(input);

    input_len = 0;
    input[0] = '\0';

    gfx_shell_print("> ");
}

void gfx_shell_print_uint(uint32_t value)
{
    char buffer[16];
    u32_to_dec_string(value, buffer);
    gfx_shell_print(buffer);
}

void gfx_shell_backspace(void) {
    if (input_len <= 0) return;

    input_len--;

    cursor_x -= CHAR_W;
    if (cursor_x < SHELL_X) {
        cursor_x = SHELL_X;
    }

    gfx_fill_rect(cursor_x, cursor_y, CHAR_W, CHAR_H, 0x303030);
}

//Redundancy used in VGA as well.
void gfx_update_hw_cursor(void) {
    uint16_t pos = (uint16_t)(cursor_y * SHELL_W + cursor_x);

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void gfx_shell_clear(uint32_t color) {
    gfx_clear(color);

    gfx_fill_rect(8, 8, 624, 464, 0x303030);
    gfx_shell_draw_rect(8, 8, 624, 464, 0xFFFFFF);

    cursor_x = SHELL_X;
    cursor_y = SHELL_Y;

    gfx_shell_print("EquineOS Graphical Shell\n");
}

void gfx_shell_panic_clear(uint32_t color) {
    gfx_clear(color);

    cursor_x = SHELL_X;
    cursor_y = SHELL_Y;
}

int vprintf(const char *fmt, va_list args)
{
    while (*fmt)
    {
        if (*fmt != '%')
        {
            gfx_shell_putchar(*fmt++);
            continue;
        }

        fmt++; // skip %

        if (*fmt == 's')
        {
            char *s = va_arg(args, char *);
            gfx_shell_print(s ? s : "(null)");
        }
        else if (*fmt == 'c')
        {
            char c = (char)va_arg(args, int);
            gfx_shell_putchar(c);
        }
        else if (*fmt == 'u')
        {
            uint32_t v = va_arg(args, uint32_t);
            gfx_shell_print_uint(v);
        }
        else if (*fmt == 'i' || *fmt == 'd')
        {
            int v = va_arg(args, int);

            if (v < 0)
            {
                gfx_shell_putchar('-');
                v = -v;
            }

            gfx_shell_print_uint((uint32_t)v);
        }
        else if (*fmt == 'x')
        {
            uint32_t v = va_arg(args, uint32_t);
            gfx_shell_print_hex32(v);
        }
        else if (*fmt == 'p')
        {
            uintptr_t v = (uintptr_t)va_arg(args, void *);
            gfx_shell_print_hex32((uint32_t)v);
        }
        else if (*fmt == '%')
        {
            gfx_shell_putchar('%');
        }

        fmt++;
    }

    return 0;
}

int printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int result = vprintf(fmt, args);

    va_end(args);
    return result;
}

static void buffer_putchar(
    char *buffer,
    size_t buffer_size,
    size_t *position,
    char c
)
{
    if (buffer_size > 0 && *position < buffer_size - 1)
        buffer[*position] = c;

    (*position)++;
}

static void buffer_puts(
    char *buffer,
    size_t buffer_size,
    size_t *position,
    const char *text
)
{
    if (!text)
        text = "(null)";

    while (*text)
        buffer_putchar(buffer, buffer_size, position, *text++);
}

static void buffer_put_uint(
    char *buffer,
    size_t buffer_size,
    size_t *position,
    uint32_t value,
    unsigned int minimum_width,
    char padding
)
{
    char digits[16];
    unsigned int length = 0;

    do
    {
        digits[length++] = (char)('0' + (value % 10));
        value /= 10;
    }
    while (value != 0);

    while (length < minimum_width)
    {
        buffer_putchar(buffer, buffer_size, position, padding);
        minimum_width--;
    }

    while (length > 0)
        buffer_putchar(buffer, buffer_size, position, digits[--length]);
}

static int vsnprintf(
    char *buffer,
    size_t buffer_size,
    const char *format,
    va_list args
)
{
    size_t position = 0;

    if (!buffer || !format)
        return -1;

    while (*format)
    {
        if (*format != '%')
        {
            buffer_putchar(
                buffer,
                buffer_size,
                &position,
                *format++
            );
            continue;
        }

        format++; /* Skip %. */

        /*
         * Doom uses formats such as %.3d.
         * For integer formatting, treat precision as minimum digits.
         */
        char padding = ' ';
        unsigned int minimum_width = 0;

        if (*format == '0')
        {
            padding = '0';
            format++;
        }

        if (*format == '.')
        {
            padding = '0';
            format++;
        }

        while (*format >= '0' && *format <= '9')
        {
            minimum_width =
                minimum_width * 10 +
                (unsigned int)(*format - '0');

            format++;
        }

        switch (*format)
        {
            case 's':
            {
                const char *text = va_arg(args, const char *);
                buffer_puts(buffer, buffer_size, &position, text);
                break;
            }

            case 'c':
            {
                int value = va_arg(args, int);
                buffer_putchar(
                    buffer,
                    buffer_size,
                    &position,
                    (char)value
                );
                break;
            }

            case 'u':
            {
                uint32_t value = va_arg(args, uint32_t);

                buffer_put_uint(
                    buffer,
                    buffer_size,
                    &position,
                    value,
                    minimum_width,
                    padding
                );
                break;
            }

            case 'd':
            case 'i':
            {
                int value = va_arg(args, int);
                uint32_t magnitude;

                if (value < 0)
                {
                    buffer_putchar(
                        buffer,
                        buffer_size,
                        &position,
                        '-'
                    );

                    /*
                     * This avoids overflowing when value == INT_MIN.
                     */
                    magnitude = (uint32_t)(-(value + 1)) + 1;
                }
                else
                {
                    magnitude = (uint32_t)value;
                }

                buffer_put_uint(
                    buffer,
                    buffer_size,
                    &position,
                    magnitude,
                    minimum_width,
                    padding
                );
                break;
            }

            case '%':
            {
                buffer_putchar(
                    buffer,
                    buffer_size,
                    &position,
                    '%'
                );
                break;
            }

            case '\0':
            {
                /*
                 * A trailing '%' is malformed. Stop safely.
                 */
                goto finished;
            }

            default:
            {
                /*
                 * Preserve unknown specifiers visibly.
                 */
                buffer_putchar(
                    buffer,
                    buffer_size,
                    &position,
                    '%'
                );

                buffer_putchar(
                    buffer,
                    buffer_size,
                    &position,
                    *format
                );
                break;
            }
        }

        format++;
    }

finished:

    if (buffer_size > 0)
    {
        size_t terminator =
            position < buffer_size ? position : buffer_size - 1;

        buffer[terminator] = '\0';
    }

    return (int)position;
}

typedef struct FILE FILE;

int fprintf(FILE *stream, const char *fmt, ...)
{
    (void)stream;

    va_list args;
    va_start(args, fmt);

    int result = vprintf(fmt, args);

    va_end(args);
    return result;
}

int putchar(int c)
{
    gfx_shell_putchar((char)c);
    return c;
}

int puts(const char *s)
{
    printf("%s\n", s);
    return 0;
}
