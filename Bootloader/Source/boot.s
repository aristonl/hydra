[org 0x7c00]

mov [BootDisk], dl

mov ah, 0x0e
mov al, '0'
int 0x10

mov bp, 0x7c00
mov sp, bp

call ReadDisk

jmp 0x7e00

%include "disk.s"

times 510-($-$$) db 0

dw 0xaa55