// isr.c
#include "isr.h"
#include "vga.h"
#include "irq.h"
#include "shell/console.h"
#include "gfx_shell.h"
#include "string.h"

//contains isr_common_handler
//contains crash/panic code.

//header

void panic(const char* msg, regs_t* r);
void vga_panic(const char* msg, regs_t* r);
void gfx_panic(const char* msg, regs_t* r);

void page_fault_handler(regs_t* r);
volatile int kernel_panicking = 0;

static const char* exception_names[32] = {
    "0 Divide Error (#DE)",
    "1 Debug (#DB)",
    "2 Non-Maskable Interrupt (NMI)",
    "3 Breakpoint (#BP)",
    "4 Overflow (#OF)",
    "5 Bound Range Exceeded (#BR)",
    "6 Invalid Opcode (#UD)",
    "7 Device Not Available (#NM)",
    "8 Double Fault (#DF)",
    "9 Coprocessor Segment Overrun (legacy)",
    "10 Invalid TSS (#TS)",
    "11 Segment Not Present (#NP)",
    "12 Stack-Segment Fault (#SS)",
    "13 General Protection Fault (#GP)",
    "14 Page Fault (#PF)",
    "15 Reserved",
    "16 x87 Floating-Point Exception (#MF)",
    "17 Alignment Check (#AC)",
    "18 Machine Check (#MC)",
    "19 SIMD Floating-Point Exception (#XM/#XF)",
    "20 Virtualization Exception (#VE)",
    "21 Control Protection Exception (#CP)",
    "22 Reserved",
    "23 Reserved",
    "24 Reserved",
    "25 Reserved",
    "26 Reserved",
    "27 Reserved",
    "28 Hypervisor Injection Exception (#HV) / Reserved on many setups",
    "29 VMM Communication Exception (#VC) / Reserved on many setups",
    "30 Security Exception (#SX)",
    "31 Reserved"
};

isr_t interrupt_handlers[256] = {0};

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void isr_common_handler(regs_t* r)
{
   if (r->int_no == 3) {
    return;
    }

    if (r->int_no == 14) {
        page_fault_handler(r);
        return;
    }

    if (r->int_no >= 32 && r->int_no < 48) {
        irq_handler(r);
        return;
    }
    panic("Unhandled exception",r);
}

void page_fault_handler(regs_t* r)
{
    uint32_t fault_addr;
    __asm__ volatile("mov %%cr2, %0" : "=r"(fault_addr));

    console_write("PAGE FAULT\n");

    console_write("Fault address: ");
    console_print_hex32(fault_addr);
    console_write("\n");

    console_write("EIP: ");
    console_print_hex32(r->eip);
    console_write("\n");

    console_write("Error code: ");
    console_print_hex32(r->err_code);
    console_write("\n");

    for (;;)
        __asm__ volatile("hlt");
}

void gfx_panic(const char* msg, regs_t* r){

    gfx_shell_panic_clear(0x000080);
    gfx_shell_print_at("==== SYSTEM HALTED ====", 0xFF4040, 28, 0);

    kernel_panicking = 1;

    if (r) {
    uint32_t n = r->int_no;
    char buf[16];
    itoa(r->int_no, buf);

    if (n < 32) {
        gfx_shell_print_at(exception_names[n], 0xFFFFFF, 11, 2);
    } else {
        gfx_shell_print_at("Unknown exception vector", 0xFFFFFF, 11, 2);
    } 

    gfx_shell_print_at("Vector:", 0xFFFFFF, 2, 5); //Int_no
    gfx_shell_print_hex32_at(r->int_no, 0xFFFFFF, 14, 5);

    gfx_shell_print_at("Error:", 0xFFFFFF, 2, 6); //Error
    gfx_shell_print_hex32_at(r->err_code, 0xFFFFFF, 14, 6);
 
    gfx_shell_print_at("At EIP:", 0xFFFFFF, 2, 7);
    gfx_shell_print_hex32_at(r->eip, 0xFFFFFF, 14, 7);

    gfx_shell_print_at("--- CPU STATE ---", 0xFFFFFF, 2, 9);

    gfx_shell_print_at("CS:", 0xFFFFFF, 2, 11);
    gfx_shell_print_hex32_at(r->cs, 0xFFFFFF, 14, 11);

    gfx_shell_print_at("EFLAGS:", 0xFFFFFF, 2, 13);
    gfx_shell_print_hex32_at(r->eflags, 0xFFFFFF, 14, 13);

    gfx_shell_print_at("EAX:", 0xFFFFFF, 2, 15); gfx_shell_print_hex32_at(r->eax, 0xFFFFFF, 14, 15);
    gfx_shell_print_at("EBX:", 0xFFFFFF, 2, 16); gfx_shell_print_hex32_at(r->ebx, 0xFFFFFF, 14, 16);
    gfx_shell_print_at("ECX:", 0xFFFFFF, 2, 17); gfx_shell_print_hex32_at(r->ecx, 0xFFFFFF, 14, 17);
    gfx_shell_print_at("EDX:", 0xFFFFFF, 2, 18); gfx_shell_print_hex32_at(r->edx, 0xFFFFFF, 14, 18);

    gfx_shell_print_at("ESI:", 0xFFFFFF, 2, 20); gfx_shell_print_hex32_at(r->esi, 0xFFFFFF, 14, 20);
    gfx_shell_print_at("EDI:", 0xFFFFFF, 2, 21); gfx_shell_print_hex32_at(r->edi, 0xFFFFFF, 14, 21);
    gfx_shell_print_at("EBP:", 0xFFFFFF, 2, 22); gfx_shell_print_hex32_at(r->ebp, 0xFFFFFF, 14, 22);
    gfx_shell_print_at("ESP:", 0xFFFFFF, 2, 23); gfx_shell_print_hex32_at(r->esp, 0xFFFFFF, 14, 23);

    }
    else {
    gfx_shell_print_at("Reason:", 0xFFFFFF, 0, 2);

    if (msg) {
        gfx_shell_print_at(msg, 0xFFFFFF, 8, 2);
    } else {
        gfx_shell_print_at("Unknown kernel panic", 0xFFFFFF, 8, 2);
    }

    gfx_shell_print_at("Source:", 0xFFFFFF, 0, 4);
    gfx_shell_print_at("Kernel", 0xFFFFFF, 8, 4);

    gfx_shell_print_at("Action:", 0xFFFFFF, 0, 5);
    gfx_shell_print_at("System halted", 0xFFFFFF, 8, 5);
    }

    gfx_shell_print_at("=== SYSTEM HALTED ===", 0xFF4040, 28, 25);

    for (;;) { __asm__ __volatile__("cli; hlt"); }
}

void panic(const char* msg, regs_t* r){

if (console_get_mode() == CONSOLE_MODE_GFX) {
    gfx_panic(msg, r);
} else {
    // VGA panic logic continues below...
    }

    vga_clear(0x4F);
    vga_print_at("==== SYSTEM HALTED ====", 0x4F, 28, 0);

    kernel_panicking = 1;

    if (r) {
    uint32_t n = r->int_no;
    if (n < 32) {
        vga_print_at(exception_names[n], 0x0C, 11, 2);
    } else {
        vga_print_at("Unknown exception vector", 0x0C, 11, 2);
    }

    vga_print_at("Vector:", 0x0F, 0, 4); //Int_no
    vga_print_hex32(r->int_no, 0x0F, 8, 4);

    vga_print_at("Error:", 0x0F, 0, 5); //Error
    vga_print_hex32(r->err_code, 0x0F, 8, 5);

    vga_print_at("At EIP:", 0x0F, 0, 6);
    vga_print_hex32(r->eip, 0x0F, 8, 6);

    vga_print_at("--- CPU STATE ---", 0x0E, 0, 8);

    vga_print_at("CS:", 0x0F, 0, 9);
    vga_print_hex32(r->cs, 0x0F, 8, 9);

    vga_print_at("EFLAGS:", 0x0F, 0, 10);
    vga_print_hex32(r->eflags, 0x0F, 8, 10);

    vga_print_at("EAX:", 0x0F, 0, 12); vga_print_hex32(r->eax, 0x0F, 8, 12);
    vga_print_at("EBX:", 0x0F, 0, 13); vga_print_hex32(r->ebx, 0x0F, 8, 13);
    vga_print_at("ECX:", 0x0F, 0, 14); vga_print_hex32(r->ecx, 0x0F, 8, 14);
    vga_print_at("EDX:", 0x0F, 0, 15); vga_print_hex32(r->edx, 0x0F, 8, 15);

    vga_print_at("ESI:", 0x0F, 20, 12); vga_print_hex32(r->esi, 0x0F, 28, 12);
    vga_print_at("EDI:", 0x0F, 20, 13); vga_print_hex32(r->edi, 0x0F, 28, 13);
    vga_print_at("EBP:", 0x0F, 20, 14); vga_print_hex32(r->ebp, 0x0F, 28, 14);
    vga_print_at("ESP:", 0x0F, 20, 15); vga_print_hex32(r->esp, 0x0F, 28, 15);
    } else {
    vga_print_at("Reason:", 0x0F, 0, 2);

    if (msg) {
        vga_print_at(msg, 0x0C, 8, 2);
    } else {
        vga_print_at("Unknown kernel panic", 0x0C, 8, 2);
    }

    vga_print_at("Source:", 0x0F, 0, 4);
    vga_print_at("Kernel", 0x0F, 8, 4);

    vga_print_at("Action:", 0x0F, 0, 5);
    vga_print_at("System halted", 0x0F, 8, 5);
    }

    vga_print_at("=== SYSTEM HALTED ===", 0x4F, 28, 20);

    for (;;) { __asm__ __volatile__("cli; hlt"); }
}
