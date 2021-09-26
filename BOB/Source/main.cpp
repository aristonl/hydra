#include "bob.hpp"
#include <stdint.h>
#include <cstring>

enum {
  EI_MAG0 = 0,       // File identification index.
  EI_MAG1 = 1,       // File identification index.
  EI_MAG2 = 2,       // File identification index.
  EI_MAG3 = 3,       // File identification index.
  EI_CLASS = 4,      // File class.
  EI_DATA = 5,       // Data encoding.
  EI_VERSION = 6,    // File version.
  EI_OSABI = 7,      // OS/ABI identification.
  EI_ABIVERSION = 8, // ABI version.
  EI_PAD = 9,        // Start of padding bytes.
  EI_NIDENT = 16     // Number of bytes in e_ident.
};

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

extern "C" unsigned long long boot(void* ImageHandle, SystemTable* SystemTable) {
  SystemTable->ConOut->Reset(SystemTable->ConOut, 1);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Better Opensource Bootloader (BOB)\r\n");
  // SystemTable->BootServices->Stall(5000000);

  // Init Filesystem
  LoadedImageProtocol* LoadedImage;
  SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageProtocolGUID, (void**)&LoadedImage);
  DevicePathProtocol* DevicePath;
  SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &DevicePathProtocolGUID, (void**)&DevicePath);
  FileSystemProtocol* Volume;
  SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &FileSystemProtocolGUID, (void**)&Volume);
  
  // Open File
  FileProtocol* FS;
  Volume->OpenVolume(Volume, &FS);
  FileProtocol* KernelFile;
  FS->Open(FS, &KernelFile, (unsigned short*) L"inferno", 0x0000000000000001, 0);

  Elf64_Ehdr header; {
    unsigned long long FileInfoSize;
    FileInfo* KernelInfo;
    KernelFile->GetInfo(KernelFile, &FileInfoGUID, FileInfoSize, (void*)0);
    SystemTable->BootServices->AllocatePool(LoaderData, FileInfoSize, (void**)&KernelInfo);
    KernelFile->GetInfo(KernelFile, &FileInfoGUID, FileInfoSize, (void**)&KernelInfo);
    unsigned long long size = sizeof(header);
    KernelFile->Read(KernelFile, &size, &header);
  }

  if (memcmp(&header.e_ident[EI_MAG0], ElfMagic, 4) != 0 || header.e_ident[EI_CLASS] != 2 || header.e_ident[EI_DATA] != 1 || header.e_type != 2 || header.e_machine != 62 || header.e_version != 1) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Error whilst loading Inferno!\r\nPlease DO NOT modify Inferno without experience!");
    SystemTable->BootServices->Stall(5000000);
    SystemTable->RuntimeServices->ResetSystem(ResetShutdown, 0x8000000000000000, 0, 0);
  }

  // Initialize Memory Map
  unsigned long long MemoryMapSize = 0, MapKey, DescriptorSize;
  MemoryDescriptor* MemoryMap;
  unsigned int DescriptorVersion;
  SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  MemoryMapSize+=2*DescriptorSize;
  SystemTable->BootServices->AllocatePool(LoaderData, MemoryMapSize, (void**)&MemoryMap);
  SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);

  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"\r\nIf all is well then this message should show up!\r\n");
  
  // Exit Boot Services
  SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, 0);
  SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
  while(1);
  return 0;
}