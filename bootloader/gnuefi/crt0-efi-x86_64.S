.text
.align 4
.globl _start
_start:
	subq $8, %rsp
	pushq %rcx
	pushq %rdx
0:
	lea ImageBase(%rip), %rdi
	lea _DYNAMIC(%rip), %rsi
	popq %rcx
	popq %rdx
	pushq %rcx
	pushq %rdx
	call _relocate
	popq %rdi
	popq %rsi
	call efi_main
	addq $8, %rsp
.exit:	
  ret
 	.data
dummy:	.long	0
 	.section .reloc, "a"
label1:
	.long	dummy-label1
 	.long	10
	.word	(IMAGE_REL_ABSOLUTE<<12) +  0

