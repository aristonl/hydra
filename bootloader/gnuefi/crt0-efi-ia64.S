.text
.psr abi64
.psr lsb
.lsb

.proc _start
_start:
	alloc loc0=ar.pfs,2,2,2,0
	mov loc1=rp
	movl out0=@gprel(ImageBase)
	add out0=out0,gp
	movl out1=@gprel(_DYNAMIC)
	add out1=out1,gp
	br.call.sptk.few rp=_relocate
.Lret0:	
	cmp.ne p6,p0=r0,r8
(p6)	br.cond.sptk.few .exit=

.Lret1:
	mov out0=in0
	mov out1=in1
	br.call.sptk.few rp=efi_main
.Lret2:
.exit:
	mov ar.pfs=loc0
	mov rp=loc1
	br.ret.sptk.few rp
	.endp _start
	.align 16
	.global _start_plabel
	.section .plabel, "a"
_start_plabel:
	data8	_start
	data8	__gp
	.section .reloc, "a"
	data4	_start_plabel
	data4	12
	data2	(IMAGE_REL_BASED_DIR64<<12) +  0
	data2	(IMAGE_REL_BASED_DIR64<<12) +  8
