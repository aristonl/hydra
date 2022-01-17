#include "Library/SystemTable.hpp"
#include "Library/FileSystem.hpp"
#include "Library/GOP.hpp"
#include "Library/Framebuffer.hpp"
#include "Library/Image.hpp"
#include "Library/Font.hpp"
#include "Library/String.hpp"
#include "Library/ELF.hpp"

Framebuffer framebuffer;

void putpixel(unsigned int x, unsigned int y, unsigned int color) {
  *(unsigned int*)((unsigned int*)framebuffer.Address + x + (y * framebuffer.PPSL)) = color;
}

void addBar() {
  for (unsigned int y=framebuffer.Height-100;y<=framebuffer.Height-100+20;y++) putpixel(100,y,0xcccccc);
  for (unsigned int x=100;x<=framebuffer.Width-100;x++) putpixel(x,framebuffer.Height-100,0xcccccc);
  for (unsigned int x=100;x<=framebuffer.Width-100;x++) putpixel(x,framebuffer.Height-100+20,0xcccccc);
  for (unsigned int y=framebuffer.Height-100;y<=framebuffer.Height-100+20;y++) putpixel(framebuffer.Width-100,y,0xcccccc);
}

void fill(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color) {
  for (int j = y; j < y+height; j++) {
    for (int i = x; i < x+width; i++) {
      putpixel(i,j,color);
    }
  }
}

void* memset(void* s, unsigned char c, unsigned long long len) {
  unsigned char *dst = (unsigned char*) s;
  while (len > 0) {
    *dst = (unsigned char) c;
    dst++;
    len--;
  }
  return s;
}

#define CheckStatus status = 

extern "C" __attribute__((ms_abi)) unsigned long long boot(void* ImageHandle, struct SystemTable* SystemTable) {
  unsigned long long status = 0;
  CheckStatus SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, 0);
  CheckStatus SystemTable->ConsoleOutput->Clear(SystemTable->ConsoleOutput);

  MemoryDescriptor* Map = 0;
	unsigned long long int MapSize, MapKey, DescriptorSize;
	unsigned int DescriptorVersion; {
		CheckStatus SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
    MapSize += 2*DescriptorSize;
		CheckStatus SystemTable->BootServices->AllocatePool(2, MapSize, (void**)&Map);
		CheckStatus SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
	}

  GUID gopGUID = GOPGUID;
  GOP* gop;
  CheckStatus SystemTable->BootServices->LocateProtocol(&gopGUID, (void*)0, (void**)&gop);
  framebuffer = Framebuffer(gop->Mode->Address, gop->Mode->Size, gop->Mode->ModeInfo->Width, gop->Mode->ModeInfo->Height, gop->Mode->ModeInfo->PPSL);

  addBar();
  LoadedImageProtocol* LoadedImage;
  CheckStatus SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageProtocolGUID, (void**)&LoadedImage);
  DevicePathProtocol* DevicePath;
  CheckStatus SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &DevicePathProtocolGUID, (void**)&DevicePath);
  FileSystemProtocol* Volume;
  CheckStatus SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &FileSystemProtocolGUID, (void**)&Volume);
  FileProtocol* FS;
  CheckStatus Volume->OpenVolume(Volume, &FS);
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/7, 19, 0xffffff);

  struct TGAImage* BootLogo;
  {
    FileProtocol* image;
    CheckStatus FS->Open(FS, &image, (unsigned short*) L"Echo.tga", 0x0000000000000001, 0);
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
    CheckStatus SystemTable->BootServices->AllocatePool(2, sizeof(struct TGAImage), (void**)&BootLogo);
    BootLogo->Pointer = header;
    BootLogo->Buffer = buffer;
    CheckStatus image->Close(image);
  }

  PrintTGA(BootLogo, SystemTable, framebuffer, framebuffer.Width/2, framebuffer.Height/2);

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

  FileProtocol* fontFile;
  CheckStatus FS->Open(FS, &fontFile, (unsigned short*) L"font.psf", 0x0000000000000001, 0);
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/6, 19, 0xffffff);

  PSFHeader* fontHeader;
  PSFFont* font;
  CheckStatus SystemTable->BootServices->AllocatePool(2, sizeof(PSFHeader), (void**)&fontHeader);
  unsigned long long fontSize = sizeof(fontHeader);
  CheckStatus fontFile->Read(fontFile, &fontSize, fontHeader);
  if (fontHeader->Magic[0] == 0x36 || fontHeader->Magic[1] == 0x04) {
    unsigned long long glyphBufferSize = fontHeader->GlyphSize*256;
    void* glyphBuffer; {
      CheckStatus fontFile->SetPosition(fontFile, sizeof(PSFHeader));
      CheckStatus SystemTable->BootServices->AllocatePool(2, glyphBufferSize, (void**)&glyphBuffer);
      CheckStatus fontFile->Read(fontFile, &glyphBufferSize, glyphBuffer);
      CheckStatus SystemTable->BootServices->AllocatePool(2, sizeof(PSFFont), (void**)&font);
      font->Header = fontHeader;
      font->GlyphBuffer = glyphBuffer;
    }
  }
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/5, 19, 0xffffff);

  FileProtocol* KernelFile;
  CheckStatus FS->Open(FS, &KernelFile, (unsigned short*) L"inferno", 0x0000000000000001, 0);
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/4, 19, 0xffffff);

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
    CheckStatus SystemTable->ConsoleOutput->Print(SystemTable->ConsoleOutput, (unsigned short int*) L"Error whilst loading Inferno!\r\nPlease DO NOT modify Inferno without experience!");
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
        memset((void*)segment, 0, pages*0x1000);
        CheckStatus KernelFile->SetPosition(KernelFile, ProgramHeader->Offset);
        unsigned long long size = ProgramHeader->FileSize;
        CheckStatus KernelFile->Read(KernelFile, &size, (void*)segment);
        break;
      }
    }
  }
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/3, 19, 0xffffff);

  ConfigurationTable* configTable = SystemTable->ConfigurationTable;
	void* rsdp = (void*)0; 
	GUID ACPI2TableGUID = ACPI20TableGUID;

	for (unsigned long long index = 0; index < SystemTable->NumberOfEntries; index++) {
		if (CompareGUID(&configTable[index].VendorGUID, &ACPI2TableGUID) && strcmp((unsigned char*)"RSD PTR ", (unsigned char*)configTable->VendorTable, 8)) rsdp = (void*)configTable->VendorTable;
		configTable++;
	}
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/2, 19, 0xffffff);

  if (status != 0) {
    fill(0, 0, framebuffer.Width, framebuffer.Height, 0);
    PrintTGA(ErrorIcon, SystemTable, framebuffer, framebuffer.Width/2, framebuffer.Height/2);
    while(1) asm("hlt");
  }

  SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
  fill(101, framebuffer.Height-99, (framebuffer.Width-200), 19, 0xffffff);

	void (*KernelMain)(Framebuffer*, PSFFont*, MemoryDescriptor*, unsigned long long int, unsigned long long int, void*)=((void (*)(Framebuffer*, PSFFont*, MemoryDescriptor*, unsigned long long int, unsigned long long int, void*))KernelHeaders.Entry);
  KernelMain(&framebuffer, font, Map, MapSize, DescriptorSize, rsdp);
  fill(0, 0, framebuffer.Width, framebuffer.Height, 0);
  PrintTGA(ErrorIcon, SystemTable, framebuffer, framebuffer.Width/2, framebuffer.Height/2);
  return 0;
}