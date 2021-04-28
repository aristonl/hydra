	.text
	.align 4

	.globl _start
_start:
        pushl %ebp
        movl %esp,%ebp
        pushl %ebx
        movl 12(%ebp),%eax
        movl 24(%eax),%ebx
        pushl %ebx
        movl 44(%eax),%ebx
        pushl %ebx
        movl 4(%ebx),%eax
        call *%eax
        movl -4(%ebp),%ebx
        leave
        ret
        pushl %ebp
        movl %esp,%ebp
        pushl %ebx
	call 0f
0:	popl %eax
	addl $hello-0b,%eax
	pushl %eax
        movl 12(%ebp),%eax
        movl 44(%eax),%ebx
        pushl %ebx
        movl 4(%ebx),%eax
        call *%eax
        movl -4(%ebp),%ebx
        leave
        ret
	.section .rodata
	.align 2
hello:  .byte 'h',0,'e',0,'l',0,'l',0,'o',0,'\n',0,'\r',0,0,0

#endif
