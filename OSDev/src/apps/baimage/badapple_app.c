#include <stdint.h>
#include "badapple.h"
#include "timer.h"

//UNCOMMONT FOR THE REAL BAD APPLE BIN
extern uint8_t _binary_src_apps_baimage_badapple_bin_start[];

//COMMENT OUT FOR BAD APPLE
//extern uint8_t _binary_src_apps_baimage_badapple_demo_bin_start[];

#define BA_SCALE 3
#define BA_OFFSET_X 20
#define BA_OFFSET_Y 40
#define BA_MAGIC_SIZE 8

extern void gfx_put_pixel(int x, int y, uint32_t color);

static uint32_t read_u32_le(uint8_t* p) {
    return ((uint32_t)p[0]) |
           ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}

static void draw_scaled_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    for (uint32_t yy = 0; yy < BA_SCALE; yy++) {
        for (uint32_t xx = 0; xx < BA_SCALE; xx++) {
            gfx_put_pixel(
                BA_OFFSET_X + x * BA_SCALE + xx,
                BA_OFFSET_Y + y * BA_SCALE + yy,
                color
            );
        }
    }
}

static void draw_frame_1bit(uint8_t* frame, uint32_t width, uint32_t height) {
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint32_t bit_index = y * width + x;
            uint32_t byte_index = bit_index / 8;
            uint32_t bit_in_byte = 7 - (bit_index % 8);

            uint8_t on = (frame[byte_index] >> bit_in_byte) & 1;

            draw_scaled_pixel(x, y, on ? 0xFFFFFFFF : 0xFF000000);
        }
    }
}

void badapple_main(void) {

    //COMMENT IN FOR BAD APPLE
    uint8_t* data = _binary_src_apps_baimage_badapple_bin_start;

    //COMMENT OUT FOR BAD APPLE
    //uint8_t* data = _binary_src_apps_baimage_badapple_demo_bin_start;

    uint32_t width       = read_u32_le(data + 8);
    uint32_t height      = read_u32_le(data + 12);
    uint32_t frame_count = read_u32_le(data + 16);

    uint32_t frame_bytes = (width * height) / 8;
    uint8_t* frames = data + 20;

    if (frame_count == 0 || width == 0 || height == 0) {
        return;
    }

    for (uint32_t i = 0; i < frame_count; i++) {
        draw_frame_1bit(frames + i * frame_bytes, width, height);
        sleep_ms(33);
    }
}
