#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

typedef struct {
	void* BaseAddress;
	size_t Size;
	unsigned int Width, Height, PixelsPerScanLine;
} Framebuffer;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;

typedef struct {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

typedef struct {
	Framebuffer* framebuffer;
	PSF1_FONT* font;
	EFI_MEMORY_DESCRIPTOR* Map;
	UINTN MapSize, MapDescriptorSize;
} BootData;

Framebuffer tempFramebuffer;
Framebuffer* InitializeGOP() {
	EFI_GUID GOP_GUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;
	EFI_STATUS status;
	status = uefi_call_wrapper(BS->LocateProtocol, 3, &GOP_GUID, NULL, (void **) &GOP);
	if (EFI_ERROR(status)) {
		Print(L"Error whilst loading GOP!\n\r");
		return NULL;
	}
	tempFramebuffer.BaseAddress = (void*) GOP->Mode->FrameBufferBase;
	tempFramebuffer.Size = GOP->Mode->FrameBufferSize;
	tempFramebuffer.Width = GOP->Mode->Info->HorizontalResolution;
	tempFramebuffer.Height = GOP->Mode->Info->VerticalResolution;
	tempFramebuffer.PixelsPerScanLine = GOP->Mode->Info->PixelsPerScanLine;
	return &tempFramebuffer;
}

EFI_FILE* LoadFile(EFI_FILE* dir, CHAR16* path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_FILE* LoadedFile;
	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**) &LoadedImage);
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**) &FileSystem);
	if (dir == NULL) FileSystem->OpenVolume(FileSystem, &dir);
	EFI_STATUS status = dir->Open(dir, &LoadedFile, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (status != EFI_SUCCESS) return NULL;
	else return LoadedFile;
}

PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_FILE* font = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if (font == NULL) return NULL;
	PSF1_HEADER* fontHeader;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)&fontHeader);
	UINTN size = sizeof(PSF1_HEADER);
	font->Read(font, &size, fontHeader);
	if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1) return NULL;
	UINTN glyphBufferSize = fontHeader->charsize * 256;
	if (fontHeader->mode == 1) glyphBufferSize = fontHeader->charsize * 512;
	void* glyphBuffer; {
		font->SetPosition(font, sizeof(PSF1_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}
	PSF1_FONT* finishedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedFont);
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	return finishedFont;
}

int memcmp(const void* aptr, const void* bptr, size_t n) {
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++) {
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	EFI_FILE* KernelFile = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);
	if (KernelFile == NULL) {
		Print(L"Error whilst loading kernel file!\n\r");
		return EFI_ABORTED;
	}
	Elf64_Ehdr header; {
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		KernelFile->GetInfo(KernelFile, &gEfiFileInfoGuid, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**) &FileInfo);
		KernelFile->GetInfo(KernelFile, &gEfiFileInfoGuid, &FileInfoSize, (void**) &FileInfo);
		UINTN size = sizeof(header);
		KernelFile->Read(KernelFile, &size, &header);
	}
	if (memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	) {
		Print(L"Error whilst reading kernel file!\n\r");
		return EFI_ABORTED;
	}
	Elf64_Phdr* phdrs; {
		KernelFile->SetPosition(KernelFile, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**) &phdrs);
		KernelFile->Read(KernelFile, &size, phdrs);
		for (Elf64_Phdr* phdr = phdrs; (char*) phdr < (char*) phdrs + header.e_phnum * header.e_phentsize; phdr = (Elf64_Phdr*)((char*) phdr + header.e_phentsize)) switch (phdr->p_type) {
			case PT_LOAD: {
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
				Elf64_Addr seg = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &seg);
				KernelFile->SetPosition(KernelFile, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				KernelFile->Read(KernelFile, &size, (void*) seg);
				break;
			}
		}
	}
	int (*KernelStart)(BootData*) = ((__attribute__((sysv_abi)) int (*)(BootData*)) header.e_entry);
	Framebuffer* framebuffer = InitializeGOP();
	PSF1_FONT* newFont = LoadPSF1Font(NULL, L"font.psf", ImageHandle, SystemTable);
	if (newFont == NULL) Print(L"Error whilst loading font file!\n\r");
	for (unsigned int y=0;y<framebuffer->Height;y++) {
		for (unsigned int x=0;x<framebuffer->Height;x++) {
			*(unsigned int*)(x + (y * framebuffer->PixelsPerScanLine * 4) + framebuffer->BaseAddress) = 0x000000;
		}
	}
	uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, ST->ConOut);
	EFI_MEMORY_DESCRIPTOR* Map = NULL;
	UINTN MapSize, MapKey, DescriptorSize;
	UINT32 DescriptorVersion; {
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**) &Map);
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
	}
	BootData bootdata;
	bootdata.framebuffer = framebuffer;
	bootdata.font = newFont;
	bootdata.Map = Map;
	bootdata.MapSize = MapSize;
	bootdata.MapDescriptorSize = DescriptorSize;
	uint8_t end = 0;
	SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
	switch (KernelStart(&bootdata)) {
		case 0: {
			Print(L"Successfully booted OS, but there was likely an error!\n\r");
			Print(L"The OS should never send you back to the bootloader\n\r");
			Print(L"Please contact Null, https://null-llc.com/support\n\r");
			end = 2;
			break;
		}
		case 1: {
			Print(L"Font file lost!\n\r");
			Print(L"Okay how did you do that?!\n\r");
			end = 0;
			break;
		}
		case 2: {
			Print(L"Framebuffer lost!\n\r");
			Print(L"Okay how did you do that?!\n\r");
			Print(L"Seriously though? HOWW?!?!\n\r");
			end = 0;
			break;
		}
		case 3: {
			Print(L"Memory Map Corrupted\n\r");
			Print(L"That shouldn't happen :P\n\r");
			Print(L"Please contact Null, https://null-llc.com/support\n\r");
			end = 2;
			break;
		}
		case 300: {
			end = 0;
			break;
		}
		case 301: {
			end = 1;
			break;
		}
		case 302: {
			end = 2;
			break;
		}
		case 303: {
			end = 3;
			break;
		}
		default: {
			Print(L"Unknown Error!\n\r");
			Print(L"We are not sure what happened...\n\r");
			Print(L"There is reason to believe that the OS is corrupted!\n\r");
			Print(L"Please contact Null, https://null-llc.com/support\n\r");
			end = 2;
			break;
		}
	}
	if (end == 0) {
		while(1) __asm__("hlt");
	} else if (end == 1) SystemTable->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
	else if (end == 2) SystemTable->RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);
	else if (end == 3) SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    
	
	return EFI_ABORTED;
}
