[org 0x7e00]

mov ah, 0x0e
mov al, '2'
int 0x10

jmp EnterProtectedMode

%include "gdt.s"

EnterProtectedMode:
  call EnableA20
  cli
  lgdt [GDT]
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  jmp CodeSegment:StartProtectedMode

EnableA20:
  in al, 0x92
  or al, 2
  out 0x92, al
  ret

[bits 32]
StartProtectedMode:
  mov ax, DataSegment
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov [0xb8000], byte '3'
  jmp $

times 2048-($-$$) db 0