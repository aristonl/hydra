#include "bob.hpp"

typedef enum EI { EI_MAG0, EI_MAG1, EI_MAG2, EI_MAG3, EI_CLASS, EI_DATA, EI_VERSION, EI_OSABI, EI_ABIVERSION, EI_PAD, EI_NIDENT } EI;

typedef struct Elf64_Ehdr {
  unsigned char e_ident[EI_NIDENT];
  unsigned long long e_type;
  unsigned long long e_machine;
  unsigned long e_version;
  unsigned long long e_entry;
  unsigned long long e_phoff;
  unsigned long long e_shoff;
  unsigned long e_flags;
  unsigned long long e_ehsize;
  unsigned long long e_phentsize;
  unsigned long long e_phnum;
  unsigned long long e_shentsize;
  unsigned long long e_shnum;
  unsigned long long e_shstrndx;
} Elf64_Ehdr;

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

  /*
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
      unsigned long FileInfoSize;
    }
  */

  // Initialize Memory Map
  unsigned long long MemoryMapSize = 0, MapKey, DescriptorSize;
  MemoryDescriptor* MemoryMap;
  unsigned int DescriptorVersion;
  SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  MemoryMapSize+=2*DescriptorSize;
  SystemTable->BootServices->AllocatePool(LoaderData, MemoryMapSize, (void**)&MemoryMap);
  SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  
  // Exit Boot Services
  SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, 0);
  SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
  while(1);
  return 0;
}