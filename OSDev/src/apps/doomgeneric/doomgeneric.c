#include <stdio.h>
#include <stdlib.h>

#include "m_argv.h"
#include "timer.h"
#include "keyboard.h"
#include "doomgeneric.h"
#include "doomheaders/doomkeys.h"
#include "serial.h"

pixel_t* DG_ScreenBuffer = NULL;
extern void gfx_put_pixel(int x, int y, uint32_t color);

void M_FindResponseFile(void);
void D_DoomMain (void);

static int FrameBufferFd = -1;
static int* FrameBuffer = 0;

static unsigned int s_PositionX = 0;
static unsigned int s_PositionY = 0;

static unsigned int s_ScreenWidth = 0;
static unsigned int s_ScreenHeight = 0;

void DG_Init(void)
{
    printf("Initializing DoomGeneric\n");
    keyboard_clear_events();

    //Other versions include timers and interrupts, but these are already called.

}

uint32_t DG_GetTicksMs(void)
{
    return timer_get_ticks_ms();
}

void DG_SleepMs(uint32_t ms)
{
    sleep_ms(ms);
}

static unsigned char translate_doom_key(
    uint8_t scancode,
    int extended
)
{
    if (extended)
    {
        switch (scancode)
        {
            case 0x48:
                return KEY_UPARROW;

            case 0x50:
                return KEY_DOWNARROW;

            case 0x4B:
                return KEY_LEFTARROW;

            case 0x4D:
                return KEY_RIGHTARROW;

            case 0x1D:
                return KEY_FIRE;   // Right Ctrl

            default:
                return 0;
        }
    }

    switch (scancode)
    {
        case 0x01:
            return KEY_ESCAPE;

        case 0x1C:
            return KEY_ENTER;

        case 0x1D:
            return KEY_FIRE;       // Left Ctrl

        case 0x39:
            return KEY_USE;

        case 0x2A:
        case 0x36:
            return KEY_RSHIFT;

        default:
            return 0;
    }
}

int DG_GetKey(int *pressed, unsigned char *doom_key)
{
    uint8_t scancode;
    int key_pressed;
    int extended;

char buffer[64];

    while (keyboard_pop_event(
        &scancode,
        &key_pressed,
        &extended
    ))
    {

        unsigned char translated =
            translate_doom_key(scancode, extended);

        if (translated == 0)
        {
            continue;
        }

        *pressed = key_pressed;
        *doom_key = translated;

        return 1;
    }

    return 0;
}

void DG_DrawFrame(void)
{
    static int shown = 0;

    if (!shown)
    {
        serial_write("DG_DrawFrame reached!\n");
        printf("DG_ScreenBuffer: %p\n", DG_ScreenBuffer);
        printf("FrameBuffer:     %p\n", FrameBuffer);
        shown = 1;
    }

    if (!DG_ScreenBuffer)
        return;

    for (int y = 0; y < DOOMGENERIC_RESY; ++y)
    {
        for (int x = 0; x < DOOMGENERIC_RESX; ++x)
        {
            uint32_t color =
                DG_ScreenBuffer[y * DOOMGENERIC_RESX + x];

            gfx_put_pixel(
                s_PositionX + x,
                s_PositionY + y,
                color
            );
        }
    }
}

void DG_SetWindowTitle(const char *title)
{
    (void)title;
}

void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;

	M_FindResponseFile();

	DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * sizeof(pixel_t));

    if (DG_ScreenBuffer == NULL)
    {
        printf("DoomGeneric: unable to allocate screen buffer\n");
        return;
    }

	DG_Init();

	D_DoomMain();
}

