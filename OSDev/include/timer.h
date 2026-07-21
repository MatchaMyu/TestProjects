#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "isr.h"

extern volatile uint32_t g_ticks;
extern volatile uint32_t g_seconds;
extern uint32_t g_timer_frequency;

void timer_init(uint32_t frequency);
void timer_callback(regs_t* regs);
void draw_uptime(void);
void sleep_ticks(uint32_t ticks);
void sleep_ms(uint32_t ms);
void sleep_test(void);

uint32_t timer_get_ticks_ms(void);
#endif
