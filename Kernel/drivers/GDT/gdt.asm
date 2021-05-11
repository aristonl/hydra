;
; Copyright Null LLC
; Please read the License!
;  _     _           _ 
; | |   | |         | |
; | |__ | |_   _  _ | | ____ ____
; |  __)| | | | |/ || |/ ___) _  |     gdt.asm
; | |   | | |_| ( (_| | |  ( ( | |     Info about the CPU
; |_|   |_|\__  |\____|_|   \_||_|
;         (____/
;

[bits 64]
LoadGDT:   
    lgdt [rdi]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop rdi
    mov rax, 0x08
    push rax
    push rdi
    retfq
GLOBAL LoadGDT