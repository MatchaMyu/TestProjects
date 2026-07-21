#include "keyboard.h"
#include "gfx_shell.h"
#include "vga.h"
#include "ports.h"
#include "shell/console.h"
#include <stdint.h>
#include "serial.h"


static int shift_pressed = 0;
static char input_buffer[256];
static int input_pos = 0;
static int input_done = 0;

#define KEYBOARD_EVENT_QUEUE_SIZE 64

typedef struct
{
    uint8_t scancode;
    uint8_t pressed;
    uint8_t extended;
} KeyboardEvent;

static KeyboardEvent keyboard_event_queue[KEYBOARD_EVENT_QUEUE_SIZE];

static volatile uint32_t keyboard_event_read = 0;
static volatile uint32_t keyboard_event_write = 0;

static void keyboard_push_event(
    uint8_t scancode,
    uint8_t pressed,
    uint8_t extended
)
{
    uint32_t next =
        (keyboard_event_write + 1) % KEYBOARD_EVENT_QUEUE_SIZE;

    /*
     * Queue full. Drop the newest event rather than overwrite
     * an event Doom has not read yet.
     */
    if (next == keyboard_event_read)
    {
        return;
    }

    keyboard_event_queue[keyboard_event_write].scancode = scancode;
    keyboard_event_queue[keyboard_event_write].pressed = pressed;
    keyboard_event_queue[keyboard_event_write].extended = extended;

    keyboard_event_write = next;
}

int keyboard_pop_event(
    uint8_t *scancode,
    int *pressed,
    int *extended
)
{
    if (keyboard_event_read == keyboard_event_write)
    {
        return 0;
    }

    KeyboardEvent event =
        keyboard_event_queue[keyboard_event_read];

    keyboard_event_read =
        (keyboard_event_read + 1) % KEYBOARD_EVENT_QUEUE_SIZE;

    if (scancode)
    {
        *scancode = event.scancode;
    }

    if (pressed)
    {
        *pressed = event.pressed;
    }

    if (extended)
    {
        *extended = event.extended;
    }

    return 1;
}

void keyboard_reset_buffer(void) {
    input_pos = 0;
    input_done = 0;
    input_buffer[0] = '\0';
}

int keyboard_input_ready(void) {
    return input_done;
}

char* keyboard_get_buffer(void) {
    return input_buffer;
}

static const char scancode_to_ascii[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,    // Ctrl
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,    // Left Shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,    // Right Shift
    '*',
    0,    // Alt
    ' ',
    0,    // Caps Lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // F1-F10
    0,    // Num Lock
    0,    // Scroll Lock
    0,    // Home
    0,    // Up
    0,    // Page Up
    '-',
    0,    // Left
    0,
    0,    // Right
    '+',
    0,    // End
    0,    // Down
    0,    // Page Down
    0,    // Insert
    0,    // Delete
    0, 0, 0,
    0,    // F11
    0     // F12
};

static char apply_shift(char c)
{
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }

    switch (c) {
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        case '-': return '_';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case '\\': return '|';
        case ';': return ':';
        case '\'': return '"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';
        case '`': return '~';
        default: return c;
    }
}

void keyboard_handler(void)
{
    static uint8_t extended_prefix = 0;

    uint8_t raw_scancode = inb(0x60);

    /*
     * Extended keys such as arrows send 0xE0 first,
     * followed by their actual scancode.
     */
    if (raw_scancode == 0xE0)
    {
        extended_prefix = 1;
        return;
    }

    uint8_t pressed =
        (raw_scancode & 0x80) == 0;

    uint8_t scancode =
        raw_scancode & 0x7F;

    uint8_t is_extended =
        extended_prefix;

    extended_prefix = 0;

    /*
     * Record both presses and releases for programs such as Doom.
     */
    keyboard_push_event(
        scancode,
        pressed,
        is_extended
    );

    /*
     * Handle Shift state for normal shell typing.
     */
    if (!is_extended &&
        (scancode == 0x2A || scancode == 0x36))
    {
        shift_pressed = pressed;
        return;
    }

    /*
     * The shell only processes ordinary key presses.
     *
     * Releases and extended keys have already been placed
     * in the event queue, so the shell can safely ignore them.
     */
    if (!pressed || is_extended)
    {
        return;
    }

    if (scancode >= 128)
    {
        return;
    }

    /*
     * Enter
     */
    if (scancode == 0x1C)
    {
        input_buffer[input_pos] = '\0';
        input_done = 1;
        console_putc('\n');
        return;
    }

    /*
     * Backspace
     */
    if (scancode == 0x0E)
    {
        if (input_pos > 0)
        {
            input_pos--;
            input_buffer[input_pos] = '\0';
            console_putc('\b');
        }

        return;
    }

    char c = scancode_to_ascii[scancode];

    if (c == 0)
    {
        return;
    }

    if (shift_pressed)
    {
        c = apply_shift(c);
    }

    if (input_pos < 255)
    {
        input_buffer[input_pos++] = c;
        input_buffer[input_pos] = '\0';
        console_putc(c);
    }
}

void keyboard_clear_events(void)
{
    keyboard_event_read = keyboard_event_write;
}
