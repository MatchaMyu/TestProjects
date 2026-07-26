#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include "multiboot.h"

int gfx_init(multiboot_info_t* mb);
void gfx_put_pixel(uint32_t x, uint32_t y, uint32_t color);
void gfx_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
void gfx_clear(uint32_t color);
void gfx_draw_char(int x, int y, char c, uint32_t color);
void gfx_draw_char_cell(int col, int row, char c, uint32_t color);
void gfx_draw_number(int x, int y, uint32_t num, uint32_t color);
void gfx_draw_string(int x, int y, const char *s, uint32_t color);

uint8_t* gfx_get_fb(void);
uint32_t gfx_get_width(void);
uint32_t gfx_get_height(void);
uint32_t gfx_get_pitch(void);
uint8_t gfx_get_bpp(void);


#endif
