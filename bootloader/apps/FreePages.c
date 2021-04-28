#include <efi.h>
#include <efilib.h>

#define MAX_NUM_PAGES 0x000000000F000000
#define MAX_ADDR ((1ULL << 46) - 1)
#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG 0

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systab) {
	EFI_STATUS efi_status;
	CHAR16 **argv;
	INTN argc = 0;
	#if DEBUG
		INTN c = 0;
	#endif
	INTN err = 0;
	INTN PgCnt = -1;
	EFI_PHYSICAL_ADDRESS PhysAddr = 0;
	InitializeLib(image, systab);
	Print(L"FreePages: __PhysAddr__ __PgCnt__\n");
	Print(L"__PhysAddr__   0... %llx\n", MAX_ADDR);
	Print(L"__PgCnt__     [0..%lx]\n", MAX_NUM_PAGES);
	Print(L"All numbers hex w/ no leading 0x\n");
	Print(L"\n");
	#if DEBUG
		Print(L"Now parse argc/argv\n");
	#endif
	argc = GetShellArgcArgv(image, &argv);
	#if DEBUG
		Print(L"argc = %d\n", argc);
	#endif
	#if DEBUG
		for (c = 0;  c < argc;  c++ ) {
			Print(L"argv[%d] = <%s>\n", c, argv[c]);
		}
	#endif
	if (argc != 3) {
		Print(L"Invalid argument count\n");
		return EFI_SUCCESS;
	}
	PhysAddr = xtoi(argv[1]);
	PgCnt = xtoi(argv[2]);
	if ((PgCnt < 0) || (PgCnt > MAX_NUM_PAGES)) {
		Print(L"Inavlid PgCnt\n");
		err++;
	}
	if (PhysAddr > MAX_ADDR) {
		Print(L"Inavlid Address\n");
		err++;
	}
	if (err) return EFI_SUCCESS;
	Print(L"FreePages(%lx,%d)\n", PhysAddr, PgCnt);
	efi_status = uefi_call_wrapper(BS->FreePages, 2, PhysAddr, PgCnt);
	if (EFI_ERROR(efi_status)) {
		Print(L"Free Pages Failed: %d\n", efi_status);
		return efi_status;
	}
	return EFI_SUCCESS;
}