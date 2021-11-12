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

extern "C" __attribute__((ms_abi)) unsigned long long boot(void* ImageHandle, struct SystemTable* SystemTable) {
  SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, 0);
  SystemTable->ConsoleOutput->ClearScreen(SystemTable->ConsoleOutput);

  GUID gopGUID = GOPGUID;
  GOP* gop;
  SystemTable->BootServices->LocateProtocol(&gopGUID, (void*)0, (void**)&gop);
  framebuffer = Framebuffer(gop->Mode->Address, gop->Mode->Size, gop->Mode->ModeInfo->Width, gop->Mode->ModeInfo->Height, gop->Mode->ModeInfo->PPSL);

  addBar();
  LoadedImageProtocol* LoadedImage;
  SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageProtocolGUID, (void**)&LoadedImage);
  DevicePathProtocol* DevicePath;
  SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &DevicePathProtocolGUID, (void**)&DevicePath);
  FileSystemProtocol* Volume;
  SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &FileSystemProtocolGUID, (void**)&Volume);
  FileProtocol* FS;
  Volume->OpenVolume(Volume, &FS);
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/10, 19, 0xffffff);
  
  FileProtocol* image;
  FS->Open(FS, &image, (unsigned short*)L"Echo.tga", 0x0000000000000001, 0);
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/9, 19, 0xffffff);

  struct TGAHeader* header;
  unsigned long long headerSize = sizeof(struct TGAHeader);
  SystemTable->BootServices->AllocatePool(2, headerSize, (void**)&header);
  image->Read(image, &headerSize, header);
  unsigned long long bufferSize = header->width*header->height*header->bbp/8;
  void* buffer; {
    image->SetPosition(image, headerSize);
    SystemTable->BootServices->AllocatePool(2, bufferSize, (void**)&buffer);
    image->Read(image, &bufferSize, buffer);
  }
  struct TGAImage* BootLogo;
  SystemTable->BootServices->AllocatePool(2, sizeof(struct TGAImage), (void**)&BootLogo);
  BootLogo->header_ptr = header;
  BootLogo->buffer = buffer;
  image->Close(image);
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/8, 19, 0xffffff);

  unsigned int* img = (unsigned int*)BootLogo->buffer;
  unsigned int height = BootLogo->header_ptr->height;
  unsigned int width = BootLogo->header_ptr->width;
  for (unsigned long long dy=0;dy<height;dy++) {
    for (unsigned long long dx=0;dx<width;dx++) {
      unsigned long long offset = dx+(height*dy);
      unsigned int color = *(img+offset);
      unsigned long long x = dx+(framebuffer.Width/2)-(width/2);
      unsigned long long y = dy+(framebuffer.Height/2)-(height/2);
      putpixel(x, y, color);
    }
  }

  FileProtocol* fontFile;
  FS->Open(FS, &fontFile, (unsigned short*) L"font.psf", 0x0000000000000001, 0);
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/7, 19, 0xffffff);

  PSFHeader* fontHeader;
  PSFFont* font;
  SystemTable->BootServices->AllocatePool(2, sizeof(PSFHeader), (void**)&fontHeader);
  unsigned long long fontSize = sizeof(fontHeader);
  fontFile->Read(fontFile, &fontSize, fontHeader);
  if (fontHeader->magic[0] == 0x36 || fontHeader->magic[1] == 0x04) {
    unsigned long long glyphBufferSize = fontHeader->charSize*256;
    void* glyphBuffer; {
      fontFile->SetPosition(fontFile, sizeof(PSFHeader));
      SystemTable->BootServices->AllocatePool(2, glyphBufferSize, (void**)&glyphBuffer);
      fontFile->Read(fontFile, &glyphBufferSize, glyphBuffer);
      SystemTable->BootServices->AllocatePool(2, sizeof(PSFFont), (void**)&font);
      font->Header = fontHeader;
      font->GlyphBuffer = glyphBuffer;
    }
  }
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/6, 19, 0xffffff);

  FileProtocol* KernelFile;
  FS->Open(FS, &KernelFile, (unsigned short*) L"inferno", 0x0000000000000001, 0);
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/5, 19, 0xffffff);

  ELFHeaders KernelHeaders; {
    unsigned long long FileInfoSize;
    FileInfo* KernelInfo;
    KernelFile->GetInfo(KernelFile, &FileInfoGUID, FileInfoSize, (void*)0);
    SystemTable->BootServices->AllocatePool(2, FileInfoSize, (void**)&KernelInfo);
    KernelFile->GetInfo(KernelFile, &FileInfoGUID, FileInfoSize, (void**)&KernelInfo);
    unsigned long long size = sizeof(KernelHeaders);
    KernelFile->Read(KernelFile, &size, &KernelHeaders);
  }

  if (memcmp(&KernelHeaders.e_ident[0], "\u007fELF\0", 4) != 0 || KernelHeaders.e_ident[4] != 2 || KernelHeaders.e_ident[5] != 1 || KernelHeaders.e_type != 2 || KernelHeaders.e_machine != 62 || KernelHeaders.e_version != 1) {
    SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short int*) L"Error whilst loading Inferno!\r\nPlease DO NOT modify Inferno without experience!");
    return 1;
  }

  ELFProgramHeaders* ProgramHeaders; {
    KernelFile->SetPosition(KernelFile, KernelHeaders.e_phoff);
    unsigned long long size = KernelHeaders.e_phnum*KernelHeaders.e_phentsize;
    SystemTable->BootServices->AllocatePool(2, size, (void**)&ProgramHeaders);
    KernelFile->Read(KernelFile, &size, ProgramHeaders);
  }

  for (ELFProgramHeaders* ProgramHeader=ProgramHeaders;(char*)ProgramHeader<(char*)ProgramHeaders+KernelHeaders.e_phnum*KernelHeaders.e_phentsize;ProgramHeader=(ELFProgramHeaders*)((char*)ProgramHeader+KernelHeaders.e_phentsize)) {
    switch (ProgramHeader->p_type) {
      case 1: {
        int pages=(ProgramHeader->p_memsz+0x1000-1)/0x1000;
        unsigned long long segment=ProgramHeader->p_paddr;
        SystemTable->BootServices->AllocatePages(AllocateAddress, 2, pages, &segment);
        KernelFile->SetPosition(KernelFile, ProgramHeader->p_offset);
        unsigned long long size = ProgramHeader->p_filesz;
        KernelFile->Read(KernelFile, &size, (void*)segment);
        break;
      }
    }
  }
  fill(101, framebuffer.Height-99, (framebuffer.Width-200)/4, 19, 0xffffff);

  MemoryDescriptor* Map = 0;
	unsigned long long int MapSize, MapKey, DescriptorSize;
	unsigned int DescriptorVersion; {
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
    MapSize += 2*DescriptorSize;
		SystemTable->BootServices->AllocatePool(2, MapSize, (void**)&Map);
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
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

  SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
  fill(101, framebuffer.Height-99, (framebuffer.Width-200), 19, 0xffffff);

	void (*KernelMain)(Framebuffer*, PSFFont*, MemoryDescriptor*, unsigned long long int, unsigned long long int, TGAImage*, void*)=((void (*)(Framebuffer*, PSFFont*, MemoryDescriptor*, unsigned long long int, unsigned long long int, TGAImage*, void*))KernelHeaders.e_entry);
  KernelMain(&framebuffer, font, Map, MapSize, DescriptorSize, BootLogo, rsdp);
  return 0;
}