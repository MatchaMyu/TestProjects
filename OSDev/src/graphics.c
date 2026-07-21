#include "graphics.h"
#include "vga.h"
#include "serial.h"
#include "font8x8.h"
#include "gfx_text.h"
#include "gfx_shell.h"

static uint8_t* fb = 0;
static uint32_t fb_width = 0;
static uint32_t fb_height = 0;
static uint32_t fb_pitch = 0;
static uint8_t fb_bpp = 0;

#define FONT_WIDTH  8
#define FONT_HEIGHT 8

#define SHELL_X 16
#define SHELL_Y 32
static int cursor_x = SHELL_X;
static int cursor_y = SHELL_Y;

int gfx_init(multiboot_info_t* mbi) {
    
    if (!(mbi->flags & MULTIBOOT_INFO_FRAMEBUFFER)) {
        serial_write("gfx_init: no framebuffer\n");
        fb = 0;
        return 0;
    }

    if (mbi->framebuffer_type != 1) {
        serial_write("gfx_init: not RGB framebuffer\n");
        fb = 0;
        return 0;
    }

    if (mbi->framebuffer_bpp != 32) {
        serial_write("gfx_init: not 32 bpp\n");
        fb = 0;
        return 0;
    }

    serial_write("gfx_init: graphics reached\n");

    fb = (uint8_t*)(uint32_t)mbi->framebuffer_addr;
    fb_width = mbi->framebuffer_width;
    fb_height = mbi->framebuffer_height;
    fb_pitch = mbi->framebuffer_pitch;
    fb_bpp = mbi->framebuffer_bpp;

    return 1;

}

void gfx_put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!fb) return;
    if (x >= fb_width || y >= fb_height) return;

        uint32_t* pixel = (uint32_t*)(fb + y * fb_pitch + x * 4);
        *pixel = color;
}

void gfx_draw_char(int x, int y, char c, uint32_t color) {
    const uint8_t *glyph = font8x8[(unsigned char)c];

    for (int row = 0; row < 8; row++) {
        uint8_t bits = glyph[row];

        for (int col = 0; col < 8; col++) {
            if (bits & (1 << col)) {
                gfx_put_pixel(x + col, y + row, color);
            }
        }
    }
}

void gfx_draw_char_cell(int col, int row, char c, uint32_t color) {
    gfx_draw_char(col * FONT_WIDTH, row * FONT_HEIGHT, c, color);
}

void gfx_draw_string(int x, int y, const char *s, uint32_t color) {
    while (*s) {
        gfx_draw_char(x, y, *s, color);
        x += 8;
        s++;
    }
}

void gfx_draw_char_scaled(int x, int y, char c, uint32_t color, int scale) {
    const uint8_t *glyph = font8x8[(unsigned char)c];

    for (int row = 0; row < 8; row++) {
        uint8_t bits = glyph[row];

        for (int col = 0; col < 8; col++) {

            if (bits & (1 << col)) {

                for (int sy = 0; sy < scale; sy++) {
                    for (int sx = 0; sx < scale; sx++) {

                        gfx_put_pixel(x + (col * scale) + sx, 
                        y + (row * scale) + sy, color);
                    }
                }

            }
        }
    }
}

void gfx_draw_string_scaled(int x, int y, const char *s, uint32_t color, int scale) {
    while (*s) {
        gfx_draw_char_scaled(x, y, *s, color, scale);

        x += 8 * scale;

        s++;
    }
}

void gfx_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t yy = y; yy < y + h; yy++) {
        for (uint32_t xx = x; xx < x + w; xx++) {
            gfx_put_pixel(xx, yy, color);
        }
    }
}

void gfx_clear(uint32_t color) {
    gfx_fill_rect(0, 0, fb_width, fb_height, color);

    cursor_x = 0;
    cursor_y = 0;
    gfx_update_hw_cursor();
}
