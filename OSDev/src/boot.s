; Multiboot v1 header + entry that calls C kernel_main
; - Entry point for the kernel when GRUB hands off control.
; - Runs in 32-bit protected mode. (Indicated by BITS 32)
; - Calls the C entry using "extern kernel_main"
; - Long term goal: Switch to long mode and jump to 64-bit entry
; - GRAPHICS MODE. This is the boot.s file that uses graphics.

BITS 32

;This is for TEXT MODE ONLY.
;SECTION .multiboot
; text mode
;align 4
;dd 0x1BADB002
;dd 0x00000003
;dd -(0x1BADB002 + 0x00000003)

SECTION .multiboot
;graphics mode
align 4
MULTIBOOT_MAGIC    equ 0x1BADB002
MULTIBOOT_FLAGS    equ 0x00000007
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

dd MULTIBOOT_MAGIC
dd MULTIBOOT_FLAGS
dd MULTIBOOT_CHECKSUM

dd 0       ; header_addr
dd 0       ; load_addr
dd 0       ; load_end_addr
dd 0       ; bss_end_addr
dd 0       ; entry_addr

dd 0       ; mode_type: 0 = linear graphics
dd 0     ; width
dd 0    ; height
dd 0      ; bits per pixel

SECTION .text
global _start
extern kernel_main ; Calls kernel_main located in file "kernel.c"

_start:
    ; clear interrupt flag
    cli

    mov [saved_magic], eax
    mov [saved_mbi], ebx

    lgdt [gdt_descriptor]

    ; call C code
    jmp 0x08:flush_cs

flush_cs:
    ; Reload data segment registers with our data selector (0x10)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up a simple stack (now using our known SS)
    mov esp, stack_top

    ; Call C kernel
    push dword [saved_mbi]
    push dword [saved_magic]
    call kernel_main

; The code above goes into kernel_main. If it returns here, the system is halted
.hang:
    hlt
    jmp .hang

SECTION .rodata
align 8
gdt_start:
    dq 0x0000000000000000        ; 0x00: null descriptor

    ; 0x08: code segment (base=0, limit=4GB), present, ring0, executable, readable
    dq 0x00CF9A000000FFFF

    ; 0x10: data segment (base=0, limit=4GB), present, ring0, writable
    dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1   ; limit
    dd gdt_start                 ; base

SECTION .bss

align 4
saved_magic:
    resd 1

saved_mbi:
    resd 1

align 16
stack_bottom:
    resb 16384            ; 16 KiB stack
align 16
stack_top:
