# MyuOS Makefile

#NOTICE: For the doom_wad1.h file, the file used is a dummy which WILL NOT EXECUTE PROPERLY.
#You can find this in the stdio.h file. If you add your own doom.wad, you can change the comment to #include it.

#BadApple ALSO uses a dummy file. You can find the makefile command here which simply changes the build location to ensure proper compiling.

ASM      = nasm
CC       = gcc
LD       = ld
GRUBMK   = grub-mkrescue

CFLAGS   = -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -nostartfiles -nodefaultlibs -Iinclude -Isrc -Isrc/apps/doomgeneric/doomheaders -nostdinc
KFLAGS   = $(CFLAGS) -g -O0 -fno-omit-frame-pointer
ASFLAGS  = -f elf32
LDFLAGS  = -m elf_i386 -T linker/linker.ld

ISO_DIR  = iso
BOOT_DIR = $(ISO_DIR)/boot
BUILD_DIR = build
BOOT_SRC = src/boot.s
KERNEL   = $(BUILD_DIR)/kernel.elf
ISO_OUT  = $(BUILD_DIR)/myuos.iso

OBJS = $(BUILD_DIR)/boot.o \
       $(BUILD_DIR)/isr.o \
       $(BUILD_DIR)/vga.o \
       $(BUILD_DIR)/idt.o \
       $(BUILD_DIR)/kernel.o \
       $(BUILD_DIR)/isr_c.o \
       $(BUILD_DIR)/irq.o \
       $(BUILD_DIR)/pic.o \
       $(BUILD_DIR)/ports.o \
       $(BUILD_DIR)/timer.o \
       $(BUILD_DIR)/keyboard.o \
       $(BUILD_DIR)/shell.o \
       $(BUILD_DIR)/commands.o \
       $(BUILD_DIR)/math.o \
       $(BUILD_DIR)/memory.o \
       $(BUILD_DIR)/heap.o \
       $(BUILD_DIR)/paging.o \
       $(BUILD_DIR)/paging_asm.o \
       $(BUILD_DIR)/string.o \
       $(BUILD_DIR)/graphics.o \
       $(BUILD_DIR)/serial.o \
       $(BUILD_DIR)/font8x8.o \
       $(BUILD_DIR)/console.o \
       $(BUILD_DIR)/gfx_shell.o \
       $(BUILD_DIR)/stdlib.o \
       $(BUILD_DIR)/stdio.o \
       $(BUILD_DIR)/compiler_rt.o \
       $(BUILD_DIR)/unistd.o \
       $(BUILD_DIR)/ctype.o \
       $(BUILD_DIR)/errno.o \
       $(BUILD_DIR)/badapple_app.o \
       $(BUILD_DIR)/badapple_bin.o \
       $(BUILD_DIR)/am_map.o \
       $(BUILD_DIR)/doomgeneric.o \
       $(BUILD_DIR)/doom_stubs.o \
       $(BUILD_DIR)/d_event.o \
       $(BUILD_DIR)/d_items.o \
       $(BUILD_DIR)/d_iwad.o \
       $(BUILD_DIR)/d_main.o \
       $(BUILD_DIR)/d_mode.o \
       $(BUILD_DIR)/doomdef.o \
       $(BUILD_DIR)/doomstat.o \
       $(BUILD_DIR)/d_net.o \
       $(BUILD_DIR)/dstrings.o \
       $(BUILD_DIR)/f_finale.o \
       $(BUILD_DIR)/f_wipe.o \
       $(BUILD_DIR)/g_game.o \
       $(BUILD_DIR)/gusconf.o \
       $(BUILD_DIR)/hu_lib.o \
       $(BUILD_DIR)/hu_stuff.o \
       $(BUILD_DIR)/i_joystick.o \
       $(BUILD_DIR)/i_endoom.o \
       $(BUILD_DIR)/i_input.o \
       $(BUILD_DIR)/info.o \
       $(BUILD_DIR)/i_sound.o \
       $(BUILD_DIR)/i_system.o \
       $(BUILD_DIR)/i_timer.o \
       $(BUILD_DIR)/i_video.o \
       $(BUILD_DIR)/m_argv.o \
       $(BUILD_DIR)/m_bbox.o \
       $(BUILD_DIR)/m_cheat.o \
       $(BUILD_DIR)/m_fixed.o \
       $(BUILD_DIR)/m_config.o \
       $(BUILD_DIR)/m_controls.o \
       $(BUILD_DIR)/m_menu.o \
       $(BUILD_DIR)/m_random.o \
       $(BUILD_DIR)/m_misc.o \
       $(BUILD_DIR)/tables.o \
       $(BUILD_DIR)/r_draw.o \
       $(BUILD_DIR)/r_data.o \
       $(BUILD_DIR)/r_things.o \
       $(BUILD_DIR)/p_ceilng.o \
       $(BUILD_DIR)/p_doors.o \
       $(BUILD_DIR)/p_enemy.o \
       $(BUILD_DIR)/p_floor.o \
       $(BUILD_DIR)/p_inter.o \
       $(BUILD_DIR)/p_lights.o \
       $(BUILD_DIR)/p_maputl.o \
       $(BUILD_DIR)/p_plats.o \
       $(BUILD_DIR)/p_spec.o \
       $(BUILD_DIR)/p_setup.o \
       $(BUILD_DIR)/p_map.o \
       $(BUILD_DIR)/p_mobj.o \
       $(BUILD_DIR)/p_pspr.o \
       $(BUILD_DIR)/p_saveg.o \
       $(BUILD_DIR)/p_sight.o \
       $(BUILD_DIR)/p_switch.o \
       $(BUILD_DIR)/p_telept.o \
       $(BUILD_DIR)/p_tick.o \
       $(BUILD_DIR)/p_user.o \
       $(BUILD_DIR)/r_bsp.o \
       $(BUILD_DIR)/r_main.o \
       $(BUILD_DIR)/r_plane.o \
       $(BUILD_DIR)/r_segs.o \
       $(BUILD_DIR)/r_sky.o \
       $(BUILD_DIR)/sha1.o \
       $(BUILD_DIR)/st_stuff.o \
       $(BUILD_DIR)/s_sound.o \
       $(BUILD_DIR)/sounds.o \
       $(BUILD_DIR)/statdump.o \
       $(BUILD_DIR)/st_lib.o \
       $(BUILD_DIR)/z_zone.o \
       $(BUILD_DIR)/d_loop.o \
       $(BUILD_DIR)/dummy.o \
       $(BUILD_DIR)/v_video.o \
       $(BUILD_DIR)/w_checksum.o \
       $(BUILD_DIR)/w_file.o \
       $(BUILD_DIR)/w_main.o \
       $(BUILD_DIR)/w_wad.o \
       $(BUILD_DIR)/wi_stuff.o \
       $(BUILD_DIR)/w_file_stdc.o

all: $(KERNEL) $(ISO_OUT)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link kernel
$(KERNEL): $(OBJS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Build ISO
$(ISO_OUT): $(KERNEL) | $(BUILD_DIR)
	cp $(KERNEL) $(BOOT_DIR)/kernel.elf
	$(GRUBMK) -o $@ $(ISO_DIR)

# Assembly sources
$(BUILD_DIR)/boot.o: $(BOOT_SRC) | $(BUILD_DIR)
	$(ASM) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/isr.o: src/isr.s | $(BUILD_DIR)
	$(ASM) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/paging_asm.o: src/paging.asm | $(BUILD_DIR)
	$(ASM) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/serial.o: src/serial.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/errno.o: src/errno.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Shell
$(BUILD_DIR)/shell.o: src/shell/shell.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Commands
$(BUILD_DIR)/commands.o: src/shell/commands.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/console.o: src/shell/console.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# C sources
$(BUILD_DIR)/kernel.o: src/kernel.c | $(BUILD_DIR)
	$(CC) $(KFLAGS) -c $< -o $@

$(BUILD_DIR)/idt.o: src/idt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/isr_c.o: src/isr.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/irq.o: src/irq.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/pic.o: src/pic.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ports.o: src/ports.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/timer.o: src/timer.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/keyboard.o: src/keyboard.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vga.o: src/vga.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/memory.o: src/memory.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/heap.o: src/heap.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/paging.o: src/paging.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/string.o: src/string.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/compiler_rt.o: src/compiler_rt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Graphics

$(BUILD_DIR)/graphics.o: src/graphics.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/font8x8.o: src/font8x8.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/gfx_shell.o: src/gfx_shell.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/stdlib.o: src/stdlib.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/math.o: src/math.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#libs
$(BUILD_DIR)/unistd.o: src/lib/unistd.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/stdio.o: src/lib/stdio.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ctype.o: src/lib/ctype.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# DOOM FILES!

$(BUILD_DIR)/am_map.o: src/apps/doomgeneric/am_map.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/d_event.o: src/apps/doomgeneric/d_event.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/d_mode.o: src/apps/doomgeneric/d_mode.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/d_net.o: src/apps/doomgeneric/d_net.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/d_iwad.o: src/apps/doomgeneric/d_iwad.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/dstrings.o: src/apps/doomgeneric/dstrings.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/doomdef.o: src/apps/doomgeneric/doomdef.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/doomgeneric.o: src/apps/doomgeneric/doomgeneric.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/doomstat.o: src/apps/doomgeneric/doomstat.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/doom_stubs.o: src/apps/doomgeneric/doom_stubs.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/d_items.o: src/apps/doomgeneric/d_items.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/d_loop.o: src/apps/doomgeneric/d_loop.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/dummy.o: src/apps/doomgeneric/dummy.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/d_main.o: src/apps/doomgeneric/d_main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/f_finale.o: src/apps/doomgeneric/f_finale.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/f_wipe.o: src/apps/doomgeneric/f_wipe.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/gusconf.o: src/apps/doomgeneric/gusconf.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hu_lib.o: src/apps/doomgeneric/hu_lib.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hu_stuff.o: src/apps/doomgeneric/hu_stuff.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/i_endoom.o: src/apps/doomgeneric/i_endoom.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/i_video.o: src/apps/doomgeneric/i_video.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/tables.o: src/apps/doomgeneric/tables.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/info.o: src/apps/doomgeneric/info.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/i_input.o: src/apps/doomgeneric/i_input.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/i_joystick.o: src/apps/doomgeneric/i_joystick.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/i_system.o: src/apps/doomgeneric/i_system.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/i_timer.o: src/apps/doomgeneric/i_timer.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/i_sound.o: src/apps/doomgeneric/i_sound.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/m_bbox.o: src/apps/doomgeneric/m_bbox.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/m_cheat.o: src/apps/doomgeneric/m_cheat.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/m_controls.o: src/apps/doomgeneric/m_controls.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/m_fixed.o: src/apps/doomgeneric/m_fixed.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/m_misc.o: src/apps/doomgeneric/m_misc.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_maputl.o: src/apps/doomgeneric/p_maputl.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_mobj.o: src/apps/doomgeneric/p_mobj.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_doors.o: src/apps/doomgeneric/p_doors.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_enemy.o: src/apps/doomgeneric/p_enemy.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_ceilng.o: src/apps/doomgeneric/p_ceilng.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_floor.o: src/apps/doomgeneric/p_floor.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_plats.o: src/apps/doomgeneric/p_plats.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_map.o: src/apps/doomgeneric/p_map.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_setup.o: src/apps/doomgeneric/p_setup.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_spec.o: src/apps/doomgeneric/p_spec.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_sight.o: src/apps/doomgeneric/p_sight.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_switch.o: src/apps/doomgeneric/p_switch.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_telept.o: src/apps/doomgeneric/p_telept.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_tick.o: src/apps/doomgeneric/p_tick.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_inter.o: src/apps/doomgeneric/p_inter.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_lights.o: src/apps/doomgeneric/p_lights.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_user.o: src/apps/doomgeneric/p_user.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_saveg.o: src/apps/doomgeneric/p_saveg.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/g_game.o: src/apps/doomgeneric/g_game.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/r_bsp.o: src/apps/doomgeneric/r_bsp.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/r_data.o: src/apps/doomgeneric/r_data.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/r_draw.o: src/apps/doomgeneric/r_draw.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/r_segs.o: src/apps/doomgeneric/r_segs.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/r_main.o: src/apps/doomgeneric/r_main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/r_plane.o: src/apps/doomgeneric/r_plane.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/r_things.o: src/apps/doomgeneric/r_things.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/r_sky.o: src/apps/doomgeneric/r_sky.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/sha1.o: src/apps/doomgeneric/sha1.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/s_sound.o: src/apps/doomgeneric/s_sound.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/sounds.o: src/apps/doomgeneric/sounds.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/statdump.o: src/apps/doomgeneric/statdump.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/st_lib.o: src/apps/doomgeneric/st_lib.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/st_stuff.o: src/apps/doomgeneric/st_stuff.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/m_config.o: src/apps/doomgeneric/m_config.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/m_menu.o: src/apps/doomgeneric/m_menu.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/m_argv.o: src/apps/doomgeneric/m_argv.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/m_random.o: src/apps/doomgeneric/m_random.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/z_zone.o: src/apps/doomgeneric/z_zone.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/v_video.o: src/apps/doomgeneric/v_video.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/w_file.o: src/apps/doomgeneric/w_file.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/w_main.o: src/apps/doomgeneric/w_main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/w_wad.o: src/apps/doomgeneric/w_wad.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/w_checksum.o: src/apps/doomgeneric/w_checksum.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/wi_stuff.o: src/apps/doomgeneric/wi_stuff.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/w_file_stdc.o: src/apps/doomgeneric/w_file_stdc.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/p_pspr.o: src/apps/doomgeneric/p_pspr.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#BA
$(BUILD_DIR)/badapple_app.o: src/apps/baimage/badapple_app.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


#NOTE: For copyright reasons, this build does NOT include the actual badapple.bin.
#You can provide your own, otherwise for now a dummy file converts.
#TO FIX: Uncomment below, comment out the dummy file, and fix badapple_app.c

#$(BUILD_DIR)/badapple_bin.o: src/apps/baimage/badapple.bin | $(BUILD_DIR)
#	objcopy -I binary -O elf32-i386 -B i386 \
#	--rename-section .data=.rodata,alloc,load,readonly,data,contents \
#	$< $@

$(BUILD_DIR)/badapple_bin.o: src/apps/baimage/badapple_demo.bin | $(BUILD_DIR)
	objcopy -I binary -O elf32-i386 -B i386 \
	--rename-section .data=.rodata,alloc,load,readonly,data,contents \
	$< $@


clean:
	rm -rf $(BUILD_DIR)

# Rebuild from scratch
re: clean all

.PHONY: all clean re

#Run
run: clean
	$(MAKE) BOOT_SRC=src/boot.s $(ISO_OUT)
	qemu-system-i386 -m 256M -cdrom $(ISO_OUT) -boot d -no-reboot -no-shutdown -serial stdio -vga std


run-text: BOOT_SRC = src/boot_text.s
run-text: clean
	$(MAKE) BOOT_SRC=src/boot_text.s $(ISO_OUT)
	qemu-system-i386 -m 256M -cdrom $(ISO_OUT) -boot d -no-reboot -no-shutdown -serial stdio -vga std

# 256M = Memory allocated. Increased to 1024 with no issues.
