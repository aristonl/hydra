#include "Library/SystemTable.hpp"
#include "Library/FileSystem.hpp"
#include "Library/GOP.hpp"
#include "Library/Framebuffer.hpp"
#include "Library/Image.hpp"
#include "Library/Font.hpp"
#include "Library/String.hpp"
#include "Library/ELF.hpp"

extern "C" __attribute__((ms_abi)) unsigned long long boot(void* ImageHandle, struct SystemTable* SystemTable) {
  SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, 0);
  SystemTable->ConsoleOutput->ClearScreen(SystemTable->ConsoleOutput);
  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short*) L"Better Opensource Bootloader (BOB)\n\r");
  
  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short*) L"Initializing FileSystem...\n\r");
  LoadedImageProtocol* LoadedImage;
  SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageProtocolGUID, (void**)&LoadedImage);
  DevicePathProtocol* DevicePath;
  SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &DevicePathProtocolGUID, (void**)&DevicePath);
  FileSystemProtocol* Volume;
  SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &FileSystemProtocolGUID, (void**)&Volume);
  FileProtocol* FS;
  Volume->OpenVolume(Volume, &FS);

  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short*) L"Initializing GOP...\n\r");
  GUID gopGUID = GOPGUID;
  GOP* gop;
  SystemTable->BootServices->LocateProtocol(&gopGUID, (void*)0, (void**)&gop);
  Framebuffer framebuffer = Framebuffer(gop->Mode->Address, gop->Mode->Size, gop->Mode->ModeInfo->Width, gop->Mode->ModeInfo->Height, gop->Mode->ModeInfo->PPSL);
  
  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short*) L"Loading Bootlogo...\n\r");
  FileProtocol* image;
  FS->Open(FS, &image, (unsigned short*)L"Echo.tga", 0x0000000000000001, 0);

  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short*) L"Getting Bootlogo data...\n\r");
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

  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short*) L"Loading Font file...\n\r");
  FileProtocol* fontFile;
  FS->Open(FS, &fontFile, (unsigned short*) L"font.psf", 0x0000000000000001, 0);

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

  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short int*) L"Loading Inferno...\r\n");
  FileProtocol* KernelFile;
  FS->Open(FS, &KernelFile, (unsigned short*) L"inferno", 0x0000000000000001, 0);

  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short int*) L"Verifying Inferno...\r\n");
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

  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short int*) L"Initializing Memory Map...\r\n");
  MemoryDescriptor* Map = 0;
	unsigned long long int MapSize, MapKey, DescriptorSize;
	unsigned int DescriptorVersion; {
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
    MapSize += 2*DescriptorSize;
		SystemTable->BootServices->AllocatePool(2, MapSize, (void**)&Map);
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
	}

  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short int*) L"Exiting BootServices...\r\n");
  SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

  SystemTable->ConsoleOutput->OutputString(SystemTable->ConsoleOutput, (unsigned short int*) L"Loading Inferno...\r\n");
  SystemTable->ConsoleOutput->ClearScreen(SystemTable->ConsoleOutput);
	void (*KernelMain)(Framebuffer*, PSFFont*, MemoryDescriptor*, unsigned long long int, unsigned long long int, TGAImage*)=((void (*)(Framebuffer*, PSFFont*, MemoryDescriptor*, unsigned long long int, unsigned long long int, TGAImage*))KernelHeaders.e_entry);
  KernelMain(&framebuffer, font, Map, MapSize, DescriptorSize, BootLogo);

  return 0;
}