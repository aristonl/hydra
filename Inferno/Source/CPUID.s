[bits 64]
hasCPUID:
  pushfq
  pushfq
  xor qword [rsp], 0x00200000
  popfq
  pushfq
  pop rax
  xor rax, [rsp]
  popfq
  and rax, 0x00200000
  ret
global hasCPUID

section .note.GNU-stack noalloc noexec nowrite progbits