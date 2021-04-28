.section	.text.head
.globl	ImageBase
ImageBase:
	.ascii	"MZ"
	.skip	58
	.long	pe_header - ImageBase
pe_header:
	.ascii	"PE"
	.short 	0
coff_header:
	.short	0x1c2
	.short	2
	.long	0
	.long	0
	.long	1
	.short	section_table - optional_header
	.short	0x306
optional_header:
	.short	0x10b
	.byte	0x02
	.byte	0x14
	.long	_edata - _start
	.long	0
	.long	0
	.long	_start - ImageBase
	.long	_start - ImageBase
	.long	0
extra_header_fields:
	.long	0
	.long	0x20
	.long	0x8
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.long	0
	.long	_edata - ImageBase
	.long	_start - ImageBase
	.long	0
	.short	EFI_SUBSYSTEM
	.short	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0x6
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
section_table:
	.ascii	".reloc"
	.byte	0
	.byte	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.short	0
	.short	0
	.long	0x42100040
	.ascii	".text"
	.byte	0
	.byte	0
	.byte	0
	.long	_edata - _start
	.long	_start - ImageBase
	.long	_edata - _start
	.long	_start - ImageBase
	.long	0
	.long	0
	.short	0
	.short	0
	.long	0xe0500020
_start:
	stmfd	sp!, {r0-r2, lr}
	mov		r2, r0
	mov		r3, r1
	adr		r1, .L_DYNAMIC
	ldr		r0, [r1]
	add		r1, r0, r1
	adr		r0, ImageBase
	bl		_relocate
	teq		r0, #0
	bne		0f
	ldmfd	sp, {r0-r1}
	bl		efi_main
0:	add		sp, sp, #12
	ldr		pc, [sp], #4
.L_DYNAMIC:
	.word	_DYNAMIC - .
