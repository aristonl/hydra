#include "bob.hpp"

static const char ElfMagic[] = {0x7f, 'E', 'L', 'F', '\0'};

int memcmp(const void* aptr, const void* bptr, unsigned long long n) {
	const unsigned char* a = (unsigned char*) aptr, *b = (unsigned char*) bptr;
	for (unsigned long long i = 0; i < n; i++) {
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

typedef unsigned long long size_t;

size_t strlen(const char *str) {
  const char* s;
  for (s=str;*s;++s);
  return (s-str);
}

struct Elf64_Ehdr {
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
  bool checkMagic() const { return (memcmp(e_ident, ElfMagic, strlen(ElfMagic))) == 0; }
  unsigned char getFileClass() const { return e_ident[4]; }
  unsigned char getDataEncoding() const { return e_ident[5]; }
};

struct Elf64_Phdr {
  unsigned int p_type;
  unsigned int p_flags;
  unsigned long long p_offset;
  unsigned long long p_vaddr;
  unsigned long long p_paddr;
  unsigned long long p_filesz;
  unsigned long long p_memsz;
  unsigned long long p_align;
};

class Framebuffer {
  public:
    Framebuffer(unsigned long long Address, size_t Size, unsigned int Width, unsigned int Height, unsigned int PPSL) {
      this->Address = (void*) Address;
      this->Size = Size;
      this->Width = Width;
      this->Height = Height;
      this->PPSL = PPSL;
    }
    void* Address;
    size_t Size;
    unsigned int Width, Height, PPSL;
};

struct TGAHeader {
	unsigned char	id_length;
	unsigned char	color_map_type;
	unsigned char	image_type;
	unsigned short color_map_index;
	unsigned short color_map_length;
	unsigned char	depth;
	unsigned short xorg;
	unsigned short yorg;
	unsigned short width;
	unsigned short height;
	unsigned char	bbp;
	unsigned char	descriptor;
} __attribute__((packed));

struct TGAImage {
	struct TGAHeader*	header_ptr;
	void* buffer;
};

typedef struct {
  unsigned char magic[2], mode, charSize;
} PSFHeader;

typedef struct {
  PSFHeader* Header;
  void* GlyphBuffer;
} PSFFont;

#define NULL ((void*)0)


extern "C" __attribute__((ms_abi)) unsigned long long boot(void* ImageHandle, SystemTable* SystemTable) {
  SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, 0);
  SystemTable->ConOut->Reset(SystemTable->ConOut, 1);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Better Opensource Bootloader (BOB)\r\n");

  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Initializing FileSystem...\r\n");
  LoadedImageProtocol* LoadedImage;
  SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageProtocolGUID, (void**)&LoadedImage);
  DevicePathProtocol* DevicePath;
  SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &DevicePathProtocolGUID, (void**)&DevicePath);
  FileSystemProtocol* Volume;
  SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &FileSystemProtocolGUID, (void**)&Volume);
  FileProtocol* FS;
  Volume->OpenVolume(Volume, &FS);

  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Initializing GOP...\r\n");
  GUID gopGUID = GOPGUID;
  GOP* gop;
  SystemTable->BootServices->LocateProtocol(&gopGUID, (void*)0, (void**)&gop);
  Framebuffer framebuffer = Framebuffer(gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize, gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution, gop->Mode->Info->PixelsPerScanLine);
  
  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Loading Bootloader logo...\r\n");
  FileProtocol* image;
  FS->Open(FS, &image, (unsigned short*) L"Echo.tga", 0x0000000000000001, 0);

  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Getting Bootloader data...\r\n");
  struct TGAHeader* header;
  unsigned long long headerSize = sizeof(struct TGAHeader);
  SystemTable->BootServices->AllocatePool(EfiLoaderData, headerSize, (void**)&header);
  image->Read(image, &headerSize, header);
  unsigned long long bufferSize = header->width*header->height*header->bbp/8;
  void* buffer; {
    image->SetPosition(image, headerSize);
    SystemTable->BootServices->AllocatePool(EfiLoaderData, bufferSize, (void**)&buffer);
    image->Read(image, &bufferSize, buffer);
  }
  struct TGAImage* BootLogo;
  SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(struct TGAImage), (void**)&BootLogo);
  BootLogo->header_ptr = header;
  BootLogo->buffer = buffer;

  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Loading Font file...\r\n");
  FileProtocol* fontFile;
  FS->Open(FS, &fontFile, (unsigned short*) L"font.psf", 0x0000000000000001, 0);

  PSFHeader* fontHeader;
  PSFFont* font;
  SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSFHeader), (void**)&fontHeader);
  unsigned long long fontSize = sizeof(fontHeader);
  fontFile->Read(fontFile, &fontSize, fontHeader);
  if (fontHeader->magic[0] == 0x36 || fontHeader->magic[1] == 0x04) {
    unsigned long long glyphBufferSize = fontHeader->charSize*256;
    void* glyphBuffer; {
      fontFile->SetPosition(fontFile, sizeof(PSFHeader));
      SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
      fontFile->Read(fontFile, &glyphBufferSize, glyphBuffer);
      SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSFFont), (void**)&font);
      font->Header = fontHeader;
      font->GlyphBuffer = glyphBuffer;
    }
  }
  
  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Loading Inferno...\r\n");
  FileProtocol* KernelFile;
  FS->Open(FS, &KernelFile, (unsigned short*) L"inferno", 0x0000000000000001, 0);

  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Verifying Inferno...\r\n");
  Elf64_Ehdr KernelHeaders; {
    unsigned long long FileInfoSize;
    FileInfo* KernelInfo;
    KernelFile->GetInfo(KernelFile, &FileInfoGUID, FileInfoSize, (void*)0);
    SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&KernelInfo);
    KernelFile->GetInfo(KernelFile, &FileInfoGUID, FileInfoSize, (void**)&KernelInfo);
    unsigned long long size = sizeof(KernelHeaders);
    KernelFile->Read(KernelFile, &size, &KernelHeaders);
  }

  if (memcmp(&KernelHeaders.e_ident[0], ElfMagic, 4) != 0 || KernelHeaders.e_ident[4] != 2 || KernelHeaders.e_ident[5] != 1 || KernelHeaders.e_type != 2 || KernelHeaders.e_machine != 62 || KernelHeaders.e_version != 1) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Error whilst loading Inferno!\r\nPlease DO NOT modify Inferno without experience!");
    SystemTable->BootServices->Stall(5000000);
    SystemTable->RuntimeServices->ResetSystem(ResetShutdown, 0x8000000000000000, 0, 0);
  }

  Elf64_Phdr* ProgramHeaders; {
    KernelFile->SetPosition(KernelFile, KernelHeaders.e_phoff);
    unsigned long long size = KernelHeaders.e_phnum*KernelHeaders.e_phentsize;
    SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&ProgramHeaders);
    KernelFile->Read(KernelFile, &size, ProgramHeaders);
  }

  for (Elf64_Phdr* ProgramHeader=ProgramHeaders;(char*)ProgramHeader<(char*)ProgramHeaders+KernelHeaders.e_phnum*KernelHeaders.e_phentsize;ProgramHeader=(Elf64_Phdr*)((char*)ProgramHeader+KernelHeaders.e_phentsize)) {
    switch (ProgramHeader->p_type) {
      case 1: {
        int pages=(ProgramHeader->p_memsz+0x1000-1)/0x1000;
        unsigned long long segment=ProgramHeader->p_paddr;
        SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);
        KernelFile->SetPosition(KernelFile, ProgramHeader->p_offset);
        unsigned long long size = ProgramHeader->p_filesz;
        KernelFile->Read(KernelFile, &size, (void*)segment);
        break;
      }
    }
  }

  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Initializing Memory Map...\r\n");
  MemoryDescriptor* Map = 0;
	unsigned long long int MapSize, MapKey, DescriptorSize;
	unsigned int DescriptorVersion; {
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
    MapSize += 2*DescriptorSize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&Map);
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
	}

  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Exiting BootServices...\r\n");
  SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Loading Inferno...\r\n");
  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	__attribute__((sysv_abi)) void (*KernelMain)(Framebuffer*, PSFFont*, MemoryDescriptor*, unsigned long long int, unsigned long long int, TGAImage*)=((__attribute__((sysv_abi)) void (*)(Framebuffer*, PSFFont*, MemoryDescriptor*, unsigned long long int, unsigned long long int, TGAImage*))KernelHeaders.e_entry);
  KernelMain(&framebuffer, font, Map, MapSize, DescriptorSize, BootLogo);

  return 0;
}