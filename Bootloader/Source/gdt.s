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
    dd GDTNull

CodeSegment equ GDTCode - GDTNull
DataSegment equ GDTData - GDTNull