OS development roadmap.
End Goal Definition: "It runs Doom."

To create an old meme, it's not an OS until it can run Doom.

Challenges: I know next to nothing about Assembly, little about C, and honestly I haven't done much of any major programming outside of school assignments which I am super quick to forget because they never feel important.

Timing is also an issue. I don't have a deadline for this. I also don't have the intention to work on this regularly enough that it'll be done in a reasonable length. Estimates put it at 1 year, and that's being generous.

Steps:

X Multiboot memory map parsing

X Basic physical memory detection

X VGA text output + cursor/backspace support

X Exception panic screen with register dump

X PIC remapping

X IRQ handling

X PIT timer display

X Timer converted to seconds/real time

X Keyboard input handling

X Basic shell with multiple commands

X Heap allocator with kmalloc/kfree

X Heap test and memory stats

X Virtual paging initialized

X Page fault handler / halt screen

X Several doomgeneric.c headers/files imports.

X Missing Libs implemented.

X !!!GENERICDOOM RUNS!!!!

LONG TERM NEXT GOAL:
Normalize the C library (w3_schools includes various functions with commands not implemented yet)
USB Identifiers (Mostly just keyboard)
Hardware Identifiers (Basic, does not have to USE them, just IDENTIFY them)
Whatis hardware command (Detects current hardware signatures)
File system
