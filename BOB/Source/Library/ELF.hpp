#ifndef ELFHeaders

struct ELFHeaders {
  unsigned char e_ident[16];
  unsigned short e_type;
  unsigned short e_machine;
  unsigned int e_version;
  unsigned long long e_entry;
  unsigned long long e_phoff;
  unsigned long long e_shoff;
  unsigned int e_flags;
  unsigned short e_ehsize;
  unsigned short e_phentsize;
  unsigned short e_phnum;
  unsigned short e_shentsize;
  unsigned short e_shnum;
  unsigned short e_shstrndx;
  bool checkMagic() const { return (memcmp(e_ident, "\u007fELF\0", strlen("\u007fELF\0"))) == 0; }
  unsigned char getFileClass() const { return e_ident[4]; }
  unsigned char getDataEncoding() const { return e_ident[5]; }
};

#endif

#ifndef ELFProgramHeaders

struct ELFProgramHeaders {
  unsigned int p_type;
  unsigned int p_flags;
  unsigned long long p_offset;
  unsigned long long p_vaddr;
  unsigned long long p_paddr;
  unsigned long long p_filesz;
  unsigned long long p_memsz;
  unsigned long long p_align;
};

#endif