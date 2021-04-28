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
	.short	0x166
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
	.long	_edata - _start
	.long	0
	.long	0
	.long	_start - ImageBase
	.long	_start - ImageBase
extra_header_fields:
	.quad	0
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

	.set		push
	.set		noreorder
	.align		4

	.globl	_start
	.ent	_start
	.type	_start, @function
_start:
	daddiu		$sp, -32
	sd		$ra, ($sp)

	// Get pc & gp
	.align		3
	bal		1f
	sd		$gp, 8($sp)
_pc:
	.dword		_gp
	.dword		_DYNAMIC
	.dword		_relocate
1:
	// pc in ra
	ld		$gp, ($ra)
	dli		$t0, _pc
	dsubu		$gp, $t0
	daddu		$gp, $ra

	sd		$a0, 16($sp)
	sd		$a1, 24($sp)

	// a2: ImageHandle
	move		$a2, $a0
	// a3: SystemTable
	move		$a3, $a1
	// a0: ImageBase
	dli		$t1, ImageBase - _pc
	daddu		$a0, $ra, $t1
	// a1: DynamicSection
	ld		$t1, 8($ra)
	dsubu		$t1, $t0
	daddu		$a1, $ra, $t1
	// call _relocate
	ld		$t1, 16($ra)
	dsubu		$t1, $t0
	daddu		$t9, $ra, $t1
	jalr		$t9
	nop
	bnez		$v0, 1b
	nop

	// a0: ImageHandle
	ld		$a0, 16($sp)
	// call efi_main
	dla		$t9, efi_main
	jalr		$t9
	// a1: SystemTable
	ld		$a1, 24($sp)

1:
	ld		$gp, 8($sp)
	ld		$ra, ($sp)
	jr		$ra
	daddiu		$sp, 32
	.end		_start

	.set		pop
