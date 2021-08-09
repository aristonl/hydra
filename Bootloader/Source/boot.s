[org 0x7c00]
mov [BootDisk], dl
mov bp, 0x7c00
mov sp, bp
mov ah, 0x02
mov bx, 0x7e00
mov al, 4
mov dl, [BootDisk]
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
int 0x13
jc error
jmp 0x7e00
BootDisk:
  db 0
error:
  jmp $
times 510-($-$$) db 0
dw 0xaa55