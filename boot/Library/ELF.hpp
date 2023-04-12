#ifndef ELFHeaders

struct ELFHeaders {
  unsigned char Identifier[16];
  unsigned short Type, Machine;
  unsigned int Version;
  unsigned long long Entry, ProgramHeaderOffset, SectionHeaderOffset;
  unsigned int Flags;
  unsigned short Size, ProgramHeaderEntrySize, ProgramHeaderEntries, SectionHeaderEntrySize, SectionHeaderEntries, SectionNameIndex;
  bool checkMagic() const { return (memcmp(Identifier, "\u007fELF\0", strlen("\u007fELF\0"))) == 0; }
  unsigned char getFileClass() const { return Identifier[4]; }
  unsigned char getDataEncoding() const { return Identifier[5]; }
};

#endif

#ifndef ELFProgramHeaders

struct ELFProgramHeaders {
  unsigned int Type, Flags;
  unsigned long long Offset, VirtualAddress, PhysicalAddress, FileSize, MemorySize, Alginment;
};

#endif