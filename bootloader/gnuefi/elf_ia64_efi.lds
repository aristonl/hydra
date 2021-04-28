OUTPUT_FORMAT("elf64-ia64-little")
OUTPUT_ARCH(ia64)
ENTRY(_start_plabel)
SECTIONS {
  . = 0;
  ImageBase = .;
  .hash : { *(.hash) }
  .gnu.hash : { *(.gnu.hash) }
  . = ALIGN(4096);
  .text : {
   _text = .;
   *(.text)
   *(.text.*)
   *(.gnu.linkonce.t.*)
   . = ALIGN(16);
  }
  _etext = .;
  _text_size = . - _text;
  . = ALIGN(4096);
  __gp = ALIGN (8) + 0x200000;
  .sdata : {
   _data = .;
   *(.got.plt)
   *(.got)
   *(.srodata)
   *(.sdata)
   *(.sbss)
   *(.scommon)
  }
  . = ALIGN(4096);
  .data : {
   *(.rodata*)
   *(.ctors)
   *(.data*)
   *(.gnu.linkonce.d*)
   *(.plabel)
   *(.dynbss)
   *(.bss)
   *(COMMON)
  }
  .note.gnu.build-id : { *(.note.gnu.build-id) }

  . = ALIGN(4096);
  .dynamic  : { *(.dynamic) }
  . = ALIGN(4096);
  .rela : {
    *(.rela.text)
    *(.rela.data*)
    *(.rela.sdata)
    *(.rela.got)
    *(.rela.gnu.linkonce.d*)
    *(.rela.stab)
    *(.rela.ctors)
  }
  _edata = .;
  _data_size = . - _etext;
  . = ALIGN(4096);
  .reloc : {
    *(.reloc)
  }
  . = ALIGN(4096);
  .dynsym   : { *(.dynsym) }
  . = ALIGN(4096);
  .dynstr   : { *(.dynstr) }
  /DISCARD/ : {
    *(.rela.plabel)
    *(.rela.reloc)
    *(.IA_64.unwind*)
    *(.IA64.unwind*)
  }
}
