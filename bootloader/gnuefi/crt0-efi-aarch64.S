.section .text.head
.globl ImageBase
ImageBase:
	.ascii	"MZ"
	.skip	58
	.long	pe_header - ImageBase
pe_header:
	.ascii	"PE"
	.short 	0
coff_header:
	.short	0xaa64
	.short	2
	.long	0
	.long	0
	.long	1
	.short	section_table - optional_header
	.short	0x206
optional_header:
	.short	0x20b
	.byte	0x02
	.byte	0x14
	.long	_data - _start
	.long	_data_size
	.long	0
	.long	_start - ImageBase
	.long	_start - ImageBase

extra_header_fields:
	.quad	0
	.long	0x1000
	.long	0x200
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
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.long	0
	.long	0x6
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
section_table:
	.ascii	".text\0\0\0"
	.long	_data - _start
	.long	_start - ImageBase
	.long	_data - _start
	.long	_start - ImageBase
	.long	0
	.long	0
	.short	0
	.short	0
	.long	0x60000020
	.ascii	".data\0\0\0"
	.long	_data_size
	.long	_data - ImageBase
	.long	_data_size
	.long	_data - ImageBase
	.long	0
	.long	0
	.short	0
	.short	0
	.long	0xc0000040
	.align		12
_start:
	stp		x29, x30, [sp, #-32]!
	mov		x29, sp

	stp		x0, x1, [sp, #16]
	mov		x2, x0
	mov		x3, x1
	adr		x0, ImageBase
	adrp		x1, _DYNAMIC
	add		x1, x1,
	bl		_relocate
	cbnz		x0, 0f
	ldp		x0, x1, [sp, #16]
	bl		efi_main
0:	ldp		x29, x30, [sp], #32
	ret
