#include "Library/SystemTable.hpp"
#include "Library/FileSystem.hpp"
#include "Library/GOP.hpp"
#include "Library/Framebuffer.hpp"
#include "Library/Image.hpp"
#include "Library/Font.hpp"
#include "Library/String.hpp"
#include "Library/ELF.hpp"

#define Debug false

void* memset(void* s, unsigned char c, unsigned long long len) {
  unsigned char *dst = (unsigned char*) s;
  while (len > 0) {
    *dst = (unsigned char) c;
    dst++;
    len--;
  }
  return s;
}

struct BOB {
	unsigned long long DescriptorSize, MapSize;
	MemoryDescriptor* MemoryMap;

  Framebuffer* Framebuffer;
};

#if Debug == true
#define CheckStatus status = 
#else
#define CheckStatus
#endif

extern "C" __attribute__((ms_abi)) unsigned long long boot(void* ImageHandle, struct SystemTable* SystemTable) {
  Framebuffer framebuffer;
  unsigned long long status = 0;
  CheckStatus SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, 0);
  CheckStatus SystemTable->ConsoleOutput->Clear(SystemTable->ConsoleOutput);

  MemoryDescriptor* Map = 0;
	unsigned long long int MapSize, MapKey, DescriptorSize;
	unsigned int DescriptorVersion; {
		CheckStatus SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
    MapSize += 2*DescriptorSize;
    while (true) {
      CheckStatus SystemTable->BootServices->AllocatePool(2, MapSize, (void**)&Map);
      CheckStatus SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
      if (status == 0) break;
      else SystemTable->BootServices->FreePool(Map);
    }
	}

  GUID gopGUID = GOPGUID;
  GOP* gop;
  CheckStatus SystemTable->BootServices->LocateProtocol(&gopGUID, (void*)0, (void**)&gop);
  framebuffer = Framebuffer(gop->Mode->Address, gop->Mode->Size, gop->Mode->ModeInfo->Width, gop->Mode->ModeInfo->Height, gop->Mode->ModeInfo->PPSL);

  LoadedImageProtocol* LoadedImage;
  CheckStatus SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageProtocolGUID, (void**)&LoadedImage);
  DevicePathProtocol* DevicePath;
  CheckStatus SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &DevicePathProtocolGUID, (void**)&DevicePath);
  FileSystemProtocol* Volume;
  CheckStatus SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &FileSystemProtocolGUID, (void**)&Volume);
  FileProtocol* FS;
  CheckStatus Volume->OpenVolume(Volume, &FS);

  struct TGAImage* ErrorIcon;
  {
    FileProtocol* image;
    CheckStatus FS->Open(FS, &image, (unsigned short*) L"error.tga", 0x0000000000000001, 0);
    struct TGAHeader* header;
    unsigned long long headerSize = sizeof(struct TGAHeader);
    CheckStatus SystemTable->BootServices->AllocatePool(2, headerSize, (void**)&header);
    CheckStatus image->Read(image, &headerSize, header);
    unsigned long long bufferSize = header->Width*header->Height*header->BytesPerPixel/8;
    void* buffer; {
      CheckStatus image->SetPosition(image, headerSize);
      CheckStatus SystemTable->BootServices->AllocatePool(2, bufferSize, (void**)&buffer);
      CheckStatus image->Read(image, &bufferSize, buffer);
    }
    CheckStatus SystemTable->BootServices->AllocatePool(2, sizeof(struct TGAImage), (void**)&ErrorIcon);
    ErrorIcon->Pointer = header;
    ErrorIcon->Buffer = buffer;
    CheckStatus image->Close(image);
  }

  FileProtocol* KernelFile;
  CheckStatus FS->Open(FS, &KernelFile, (unsigned short*) L"kernel", 0x0000000000000001, 0);

  ELFHeaders KernelHeaders; {
    unsigned long long FileInfoSize;
    FileInfo* KernelInfo;
    CheckStatus KernelFile->GetInfo(KernelFile, &FileInfoGUID, FileInfoSize, (void*)0);
    CheckStatus SystemTable->BootServices->AllocatePool(2, FileInfoSize, (void**)&KernelInfo);
    CheckStatus KernelFile->GetInfo(KernelFile, &FileInfoGUID, FileInfoSize, (void**)&KernelInfo);
    unsigned long long size = sizeof(KernelHeaders);
    CheckStatus KernelFile->Read(KernelFile, &size, &KernelHeaders);
  }

  if (memcmp(&KernelHeaders.Identifier[0], "\u007fELF\0", 4) != 0 || KernelHeaders.Identifier[4] != 2 || KernelHeaders.Identifier[5] != 1 || KernelHeaders.Type != 2 || KernelHeaders.Machine != 62 || KernelHeaders.Version != 1) {
    PrintTGA(ErrorIcon, SystemTable, framebuffer, framebuffer.Width/2, framebuffer.Height/2);
    CheckStatus SystemTable->ConsoleOutput->Print(SystemTable->ConsoleOutput, (unsigned short int*) L"Error whilst loading kernel!\r\n");
    return 1;
  }

  ELFProgramHeaders* ProgramHeaders; {
    CheckStatus KernelFile->SetPosition(KernelFile, KernelHeaders.ProgramHeaderOffset);
    unsigned long long size = KernelHeaders.ProgramHeaderEntries*KernelHeaders.ProgramHeaderEntrySize;
    CheckStatus SystemTable->BootServices->AllocatePool(2, size, (void**)&ProgramHeaders);
    CheckStatus KernelFile->Read(KernelFile, &size, ProgramHeaders);
  }

  for (ELFProgramHeaders* ProgramHeader=ProgramHeaders;(char*)ProgramHeader<(char*)ProgramHeaders+KernelHeaders.ProgramHeaderEntries*KernelHeaders.ProgramHeaderEntrySize;ProgramHeader=(ELFProgramHeaders*)((char*)ProgramHeader+KernelHeaders.ProgramHeaderEntrySize)) {
    switch (ProgramHeader->Type) {
      case 1: {
        int pages=(ProgramHeader->MemorySize+0x1000-1)/0x1000;
        unsigned long long segment=ProgramHeader->PhysicalAddress;
        memset((void*)segment, 0, pages*4096+1);
        CheckStatus KernelFile->SetPosition(KernelFile, ProgramHeader->Offset);
        unsigned long long size = ProgramHeader->FileSize;
        CheckStatus KernelFile->Read(KernelFile, &size, (void*)segment);
        break;
      }
    }
  }

  ConfigurationTable* configTable = SystemTable->ConfigurationTable;
	void* rsdp = (void*)0; 
	GUID ACPI2TableGUID = ACPI20TableGUID;

	for (unsigned long long index = 0; index < SystemTable->NumberOfEntries; index++) {
		if (CompareGUID(&configTable[index].VendorGUID, &ACPI2TableGUID) && strcmp((unsigned char*)"RSD PTR ", (unsigned char*)configTable->VendorTable, 8)) rsdp = (void*)configTable->VendorTable;
		configTable++;
	}

  if (status != 0) {
    PrintTGA(ErrorIcon, SystemTable, framebuffer, framebuffer.Width/2, framebuffer.Height/2);
    while(1) asm("hlt");
  }

  SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

	BOB* bob;
  bob->DescriptorSize = DescriptorSize;
  bob->MapSize = MapSize;
	bob->MemoryMap = Map;
  bob->Framebuffer = &framebuffer;

	void (*KernelMain)(BOB*)=((void (*)(BOB*))KernelHeaders.Entry);
  KernelMain(bob);
  PrintTGA(ErrorIcon, SystemTable, framebuffer, framebuffer.Width/2, framebuffer.Height/2);
  return 0;
}
