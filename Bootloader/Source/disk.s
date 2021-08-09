ReadDisk:
  mov ah, 0x02
  mov bx, 0x7e00
  mov al, 4
  mov dl, [BootDisk]
  mov ch, 0x00
  mov dh, 0x00
  mov cl, 0x02
  int 0x13
  jc error
  ret

BootDisk:
  db 0

error:
  mov ah, 0x0e
  mov al, '1'
  int 0x10
  jmp $