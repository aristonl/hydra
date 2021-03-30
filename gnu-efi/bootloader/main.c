#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

typedef struct {
	void* BaseAddress;
	size_t Size;
	unsigned int Width, Height, PixelsPerScanLine;
} Framebuffer;

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
	void (*KernelStart)(Framebuffer*) = ((__attribute__((sysv_abi)) void (*)(Framebuffer*)) header.e_entry);
	// Change output type to custom exit codes for reboot, shutdown, sleep, and panic
	Framebuffer* framebuffer = InitializeGOP();
	KernelStart(framebuffer);
	return EFI_SUCCESS;
}
