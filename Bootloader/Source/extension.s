jmp EnterProtectedMode
GDTNull:
  dd 0
  dd 0
GDTCode:
  dw 0xFFFF
  dw 0x0000
  db 0x00
  db 10011010b
  db 11001111b
  db 0x00
GDTData:
  dw 0xFFFF
  dw 0x0000
  db 0x00
  db 10010010b
  db 11001111b
  db 0x00
GDTEnd:
GDT:
  GDTSize:
    dw GDTEnd - GDTNull - 1
    dq GDTNull
CodeSegment equ GDTCode - GDTNull
DataSegment equ GDTData - GDTNull
[bits 32]
Set64BitGDT:
  mov [GDTCode + 6], byte 10101111b
  mov [GDTCode + 6], byte 10101111b
  ret
[bits 16]
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
  pushfd
  pop eax
  mov ecx, eax
  xor eax, 1 << 21
  push eax
  popfd
  pushfd
  pop eax
  push ecx
  popfd
  xor eax, ecx
  jz NoCPUID
  mov eax, 0x80000001
  cpuid
  test edx, 1 << 29
  jz NoLongMode
  mov edi, 0x1000
  mov cr3, edi
  mov dword[edi], 0x2003
  add edi, 0x1000
  mov dword[edi], 0x3003
  add edi, 0x1000
  mov dword[edi], 0x4003
  add edi, 0x1000
  mov ebx, 0x00000003
  mov ecx, 512
  .SetEntry:
    mov dword[edi], ebx
    add ebx, 0x1000
    add edi, 8
    loop .SetEntry
  mov eax, cr4
  or eax, 1 << 5
  mov cr4, eax
  mov ecx, 0xC0000080
  rdmsr
  or eax, 1 << 8
  wrmsr
  mov eax, cr0
  or eax, 1 << 31
  mov cr0, eax
  call Set64BitGDT
  jmp CodeSegment:Start64Bit
NoLongMode:
  mov [0xb8000], byte 'N'
  mov [0xb8001], byte 0x1F
  mov [0xb8002], byte 'o'
  mov [0xb8003], byte 0x1F
  mov [0xb8004], byte ' '
  mov [0xb8005], byte 0x1F
  mov [0xb8006], byte 'L'
  mov [0xb8007], byte 0x1F
  mov [0xb8008], byte 'o'
  mov [0xb8009], byte 0x1F
  mov [0xb800A], byte 'n'
  mov [0xb800B], byte 0x1F
  mov [0xb800C], byte 'g'
  mov [0xb800D], byte 0x1F
  mov [0xb800E], byte ' '
  mov [0xb800F], byte 0x1F
  mov [0xb8010], byte 'M'
  mov [0xb8011], byte 0x1F
  mov [0xb8012], byte 'o'
  mov [0xb8013], byte 0x1F
  mov [0xb8014], byte 'd'
  mov [0xb8015], byte 0x1F
  mov [0xb8016], byte 'e'
  mov [0xb8017], byte 0x1F
  hlt
NoCPUID:
  mov [0xb8000], byte 'N'
  mov [0xb8001], byte 0x1F
  mov [0xb8002], byte 'o'
  mov [0xb8003], byte 0x1F
  mov [0xb8004], byte ' '
  mov [0xb8005], byte 0x1F
  mov [0xb8006], byte 'C'
  mov [0xb8007], byte 0x1F
  mov [0xb8008], byte 'P'
  mov [0xb8009], byte 0x1F
  mov [0xb800A], byte 'U'
  mov [0xb800B], byte 0x1F
  mov [0xb800C], byte 'I'
  mov [0xb800D], byte 0x1F
  mov [0xb800E], byte 'D'
  mov [0xb800F], byte 0x1F
  hlt
[bits 64]
Start64Bit:
  jmp $
times 2048-($-$$) db 0