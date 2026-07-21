// isr.h
#pragma once
#include <stdint.h>

typedef struct regs {
    uint32_t gs, fs, es, ds;          // pushed manually
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
    uint32_t int_no;                  // vector number
    uint32_t err_code;                // error code (real or fake 0)
    uint32_t eip, cs, eflags;          // pushed by CPU automatically
    uint32_t useresp, ss;              // only meaningful if coming from ring3->ring0
} regs_t;

void isr_common_handler(regs_t* r);
void isr_install(void);
typedef void (*isr_t)(regs_t*);
extern isr_t interrupt_handlers[256];
void register_interrupt_handler(uint8_t n, isr_t handler);
void page_fault_handler(regs_t* r);

void panic(const char* msg, regs_t* r); // Free to remove when adding a panic.c
void gfx_panic(const char* msg, regs_t* r); 
extern volatile int kernel_panicking; //Also free to remove when adding a panic.c
