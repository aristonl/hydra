#include "bob.hpp"
#include <stdint.h>
#include <cstring>

enum { EI_MAG0 = 0, EI_MAG1 = 1, EI_MAG2 = 2, EI_MAG3 = 3, EI_CLASS = 4, EI_DATA = 5, EI_VERSION = 6, EI_OSABI = 7, EI_ABIVERSION = 8, EI_PAD = 9, EI_NIDENT = 16 };

using Elf64_Addr = uint64_t;
using Elf64_Off = uint64_t;
using Elf64_Half = uint16_t;
using Elf64_Word = uint32_t;
using Elf64_Sword = int32_t;
using Elf64_Xword = uint64_t;
using Elf64_Sxword = int64_t;

static const char ElfMagic[] = {0x7f, 'E', 'L', 'F', '\0'};

int memcmp(const void* aptr, const void* bptr, unsigned long long n) {
	const unsigned char* a = (unsigned char*) aptr, *b = (unsigned char*) bptr;
	for (unsigned long long i = 0; i < n; i++) {
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

struct Elf64_Ehdr {
  unsigned char e_ident[EI_NIDENT];
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;
  Elf64_Off e_phoff;
  Elf64_Off e_shoff;
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
  bool checkMagic() const { return (memcmp(e_ident, ElfMagic, strlen(ElfMagic))) == 0; }
  unsigned char getFileClass() const { return e_ident[EI_CLASS]; }
  unsigned char getDataEncoding() const { return e_ident[EI_DATA]; }
};

struct Elf64_Phdr {
  Elf64_Word p_type;
  Elf64_Word p_flags;
  Elf64_Off p_offset;
  Elf64_Addr p_vaddr;
  Elf64_Addr p_paddr;
  Elf64_Xword p_filesz;
  Elf64_Xword p_memsz;
  Elf64_Xword p_align;
};

void itoa(unsigned long int n, unsigned short int* buffer, int basenumber) {
  unsigned long int hold = n;
  int i=0, j;
  do {
    hold = n % basenumber;
    buffer[i++] = (hold<10) ? (hold+'0') : (hold+'a'-10);
  } while (n /= basenumber);
  buffer[i--] = 0;
  for (j=0;j<i;j++,i--) {
    hold=buffer[j];
    buffer[j] = buffer[i];
    buffer[i] = hold;
  }
}

void* memcpy(void* dst, const void* src, unsigned long long size) {
  for (unsigned long long i=0;i<size;i++) ((unsigned char*) dst)[i] = ((const unsigned char*) src)[i];
  return dst;
}

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

extern "C" unsigned long long boot(void* ImageHandle, SystemTable* SystemTable) {
  SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, 0);
  SystemTable->ConOut->Reset(SystemTable->ConOut, 1);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Better Opensource Bootloader (BOB)\r\n");

  // Init Filesystem
  LoadedImageProtocol* LoadedImage;
  SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageProtocolGUID, (void**)&LoadedImage);
  DevicePathProtocol* DevicePath;
  SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &DevicePathProtocolGUID, (void**)&DevicePath);
  FileSystemProtocol* Volume;
  SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &FileSystemProtocolGUID, (void**)&Volume);
  FileProtocol* FS;
  Volume->OpenVolume(Volume, &FS);

  // Init Graphics Output Protocol
  GUID gopGUID = GOPGUID;
  GOP* gop;
  SystemTable->BootServices->LocateProtocol(&gopGUID, (void*)0, (void**)&gop);
  Framebuffer framebuffer = Framebuffer(gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize, gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution, gop->Mode->Info->PixelsPerScanLine);
  
  // Load Boot Logo
  FileProtocol* image;
  FS->Open(FS, &image, (unsigned short*) L"Echo.tga", 0x0000000000000001, 0);

  struct TGAHeader* header;
  unsigned long long headerSize = sizeof(struct TGAHeader);
  SystemTable->BootServices->AllocatePool(LoaderData, headerSize, (void**)&header);
  image->Read(image, &headerSize, header);
  unsigned long long bufferSize = header->width*header->height*header->bbp/8;
  void* buffer; {
    image->SetPosition(image, headerSize);
    SystemTable->BootServices->AllocatePool(LoaderData, bufferSize, (void**)&buffer);
    image->Read(image, &bufferSize, buffer);
  }
  struct TGAImage* BootLogo;
  SystemTable->BootServices->AllocatePool(LoaderData, sizeof(struct TGAImage), (void**)&BootLogo);
  BootLogo->header_ptr = header;
  BootLogo->buffer = buffer;

  // Draw Boot Logo
  unsigned int* pixel = (unsigned int*)framebuffer.Address;
  for (unsigned int y=0;y<=framebuffer.Height;y++) {
    for (unsigned int x=0;x<=framebuffer.Width;x++) {
      *(unsigned int*)(pixel + x + (y * framebuffer.PPSL)) = 0;
    }
  }
  unsigned int* img = (unsigned int*)BootLogo->buffer;
  unsigned int height = BootLogo->header_ptr->height;
  unsigned int width = BootLogo->header_ptr->width;
  for (size_t dy=0;dy<height;dy++) {
    for (size_t dx=0;dx<width;dx++) {
      size_t offset = dx+(height*dy);
      unsigned int color = *(img+offset);
      size_t x = dx+(framebuffer.Width/2)-(width/2);
      size_t y = dy+(framebuffer.Height/2)-(height/2);
      *(unsigned int*)(pixel+x+(y*framebuffer.PPSL)) = color;
    }
  }

  // Load Kernel File
  FileProtocol* KernelFile;
  FS->Open(FS, &KernelFile, (unsigned short*) L"inferno", 0x0000000000000001, 0);

  // Verify Kernel
  Elf64_Ehdr KernelHeaders; {
    unsigned long long FileInfoSize;
    FileInfo* KernelInfo;
    KernelFile->GetInfo(KernelFile, &FileInfoGUID, FileInfoSize, (void*)0);
    SystemTable->BootServices->AllocatePool(LoaderData, FileInfoSize, (void**)&KernelInfo);
    KernelFile->GetInfo(KernelFile, &FileInfoGUID, FileInfoSize, (void**)&KernelInfo);
    unsigned long long size = sizeof(KernelHeaders);
    KernelFile->Read(KernelFile, &size, &KernelHeaders);
  }

  if (memcmp(&KernelHeaders.e_ident[EI_MAG0], ElfMagic, 4) != 0 || KernelHeaders.e_ident[EI_CLASS] != 2 || KernelHeaders.e_ident[EI_DATA] != 1 || KernelHeaders.e_type != 2 || KernelHeaders.e_machine != 62 || KernelHeaders.e_version != 1) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Error whilst loading Inferno!\r\nPlease DO NOT modify Inferno without experience!");
    SystemTable->BootServices->Stall(5000000);
    SystemTable->RuntimeServices->ResetSystem(ResetShutdown, 0x8000000000000000, 0, 0);
  }

  Elf64_Phdr* ProgramHeaders; {
    KernelFile->SetPosition(KernelFile, KernelHeaders.e_phoff);
    unsigned long long size = KernelHeaders.e_phnum*KernelHeaders.e_phentsize;
    SystemTable->BootServices->AllocatePool(LoaderData, size, (void**)&ProgramHeaders);
    KernelFile->Read(KernelFile, &size, ProgramHeaders);
  }

  for (Elf64_Phdr* ProgramHeader=ProgramHeaders;(char*)ProgramHeader<(char*)ProgramHeaders+KernelHeaders.e_phnum*KernelHeaders.e_phentsize;ProgramHeader=(Elf64_Phdr*)((char*)ProgramHeader+KernelHeaders.e_phentsize)) {
    switch (ProgramHeader->p_type) {
      case 1: {
        int pages=(ProgramHeader->p_memsz+0x1000-1)/0x1000;
        Elf64_Addr segment=ProgramHeader->p_paddr;
        SystemTable->BootServices->AllocatePages(AllocateAddress, LoaderData, pages, &segment);
        KernelFile->SetPosition(KernelFile, ProgramHeader->p_offset);
        unsigned long long size = ProgramHeader->p_filesz;
        KernelFile->Read(KernelFile, &size, (void*)segment);
        break;
      }
    }
  }

  // Initialize Memory Map
  unsigned long long MemoryMapSize = 0, MapKey, DescriptorSize;
  MemoryDescriptor* MemoryMap;
  unsigned int DescriptorVersion;
  SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  MemoryMapSize+=2*DescriptorSize;
  SystemTable->BootServices->AllocatePool(LoaderData, MemoryMapSize, (void**)&MemoryMap);
  SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);

  // Exit Boot Services
  SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

  // Load Kernel
	void (*KernelMain)(Framebuffer*)=((__attribute__((ms_abi)) void (*)(Framebuffer*))KernelHeaders.e_entry);
  KernelMain(&framebuffer);

  SystemTable->RuntimeServices->ResetSystem(ResetShutdown, 0x8000000000000000, 0, 0);
  return 0;
}