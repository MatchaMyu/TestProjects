#include "shell.h"
#include "vga.h"
#include "keyboard.h"
#include "commands.h"
#include "gfx_shell.h"
#include "isr.h"

void draw_uptime(void);

int shell_row = 8; //This will need to be changed

int shell_active = 1;

void shell_set_active(int active)
{
    shell_active = active;
}

int shell_is_active(void)
{
    return shell_active;
}

//For clear command
void shell_clear(void) {
    for (int row = 8; row <= 23; row++) {
        for (int col = 0; col < 80; col++) {
            vga_print_at(" ", 0x0F, col, row);
        }
    }

    shell_row = 8;
    vga_set_cursor_pos(2, shell_row);
}
//End Clear command


//Scrolling
void shell_scroll(void) {
    volatile unsigned short *vga = (volatile unsigned short *)0xB8000;

    for (int row = 8; row < 23; row++) {
        for (int col = 0; col < 80; col++) {
            vga[row * 80 + col] = vga[(row + 1) * 80 + col];
        }
    }

    for (int col = 0; col < 80; col++) {
        vga[23 * 80 + col] = (0x0F << 8) | ' ';
    }
}
//End Scrolling

void shell_print(const char *text) {
    vga_print(text);

    //shell_row = vga_get_cursor_y();

    if (shell_row > 23) {
        shell_scroll();
        shell_row = 23;
    }
}

void shell_run(void) {
    while (1) {

        if (shell_row > 22) {
            shell_scroll();
	    shell_row = 22;
        }

	keyboard_reset_buffer();

        vga_print_at("> ", 0x0F, 0, shell_row);
        vga_set_cursor_pos(2, shell_row);

        while (!keyboard_input_ready())  {
	__asm__ __volatile__("hlt");
        }
	shell_row++;
	vga_set_cursor_pos(0, shell_row);

	process_command(keyboard_get_buffer());

	shell_row = vga_get_cursor_y();

	if (vga_get_cursor_x() != 0) {
    	shell_row++;
		}
	}

}
