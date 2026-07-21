#include <stdint.h>

#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_handler(void);
void keyboard_reset_buffer(void);
int keyboard_input_ready(void);
char* keyboard_get_buffer(void);
int keyboard_pop_event(uint8_t *scancode, int *pressed, int *extended);
void keyboard_clear_events(void);

#endif
