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

X Memory Management (memcpy, memset)

X String management (strcpy, strlen)

X Graphic settings enabled

X Bootable in graphic OR text depending on boot startup.

X Timer fixed to work with graphics AND VGA

X Bad apple runs! (Will not be in final build)

NEXT:

Hardware Identifiers (Basic, does not have to USE them, just IDENTIFY them)
DOOM Port. (GOAL. genericdoom)

LONG TERM:
Whatis hardware command (Detects current hardware signatures)
File system
Super Mario Brothers

What is NOT needed:
These would make things too complex.

Advanced GUI is a bit much. Command prompt similar style screen works, so long as it can launch DOOM. (Many OSes are capable of doing this without a GUI)
