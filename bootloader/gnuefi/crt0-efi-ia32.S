.text
.align 4
.globl _start
_start:
	pushl %ebp
	movl %esp,%ebp
	pushl 12(%ebp)
	pushl  8(%ebp)
	call 0f
0:	popl %eax
	movl %eax,%ebx
	addl $ImageBase-0b,%eax
	addl $_DYNAMIC-0b,%ebx
	pushl %ebx
	pushl %eax
	call _relocate
	popl %ebx
	popl %ebx
 	testl %eax,%eax
 	jne .exit
  call efi_main
.exit:	leave
  ret
 	.data
dummy:	.long	0
 	.section .reloc
 	.long	dummy
 	.long	10
 	.word	(IMAGE_REL_ABSOLUTE<<12) +  0
