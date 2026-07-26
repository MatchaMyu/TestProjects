#include "pic.h"
#include "ports.h"

void pic_remap(int offset1, int offset2) {

static uint8_t pic1_mask;
static uint8_t pic2_mask;

pic1_mask = inb(PIC1_DATA);
pic2_mask = inb(PIC2_DATA);

uint8_t a1 = pic1_mask;
uint8_t a2 = pic2_mask;

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, offset1);
    io_wait();
    outb(PIC2_DATA, offset2);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);

    //Debug for keyboard
    outb(PIC1_DATA, 0xFC);
    outb(PIC2_DATA, 0xFF);

}

//Getters for debugging
uint8_t pic_get_pic1_mask(void) { return inb(PIC1_DATA); }

uint8_t pic_get_pic2_mask(void) { return inb(PIC2_DATA); }

void pic_send_eoi(unsigned char irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, 0x20);
    }
    outb(PIC1_COMMAND, 0x20);
}
