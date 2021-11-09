global FlushTSS
FlushTSS:
  mov ax, (5 * 8) | 0
  ltr ax
  ret