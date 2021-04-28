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
	INTN argc;
	INTN err = 0;
	#if DEBUG
		INTN c = 0;
	#endif
	INTN AllocType = -1;
	INTN MemType = -1;
	INTN NumPages = -1;
	EFI_PHYSICAL_ADDRESS Addr = 0;
	InitializeLib(image, systab);
	Print(L"AllocatePage: __AllocType__ __MemType__ __NumPages__ [__Addr__]\n");
	Print(L"__AllocType__ {0,1,2} -- Any, MaxAddr, Addr\n"); 
	Print(L"__MemType__   {0..13}, Reserved ==0, LCode==1, LData==2, BSCode==3, BSData==4, ...\n");
	Print(L"__NumPages__  {0..%x}\n", MAX_NUM_PAGES);
	Print(L"[__Addr__]     0... %llx\n", MAX_ADDR);
	Print(L"All numbers in hex no leading 0x\n");
	Print(L"\n");
	#if DEBUG
		Print(L"Now get argc/argv\n");
	#endif
	argc = GetShellArgcArgv(image, &argv);
	#if DEBUG
		Print(L"argc = %d\n", argc);
	#endif

	#if DEBUG
		for (c = 0;  c < argc;  c++ ) Print(L"argv[%d] = <%s>\n", c, argv[c]);
	#endif
	if ((argc < 4) || (argc > 5)) {
		Print(L"Wrong argument count\n");
		return EFI_SUCCESS;
	}
	AllocType = xtoi(argv[1]);
	MemType = xtoi(argv[2]);
	NumPages = xtoi(argv[3]);
	if (argc == 5) Addr = xtoi(argv[4]);
	if ((AllocType < 0) || (AllocType > 2)) {
		Print(L"Invalid AllocType\n");
		err++;
	}
	if ((MemType < 0) || (MemType > 13)) {
		Print(L"Invalid MemType\n");
		err++;
	}
	if ((NumPages < 0) || (NumPages > MAX_NUM_PAGES)) {
		Print(L"Inavlid NumPages\n");
		err++;
	}
	if (Addr > MAX_ADDR) {
		Print(L"Inavlid Address\n");
		err++;
	}
	if (err) return EFI_INVALID_PARAMETER;
	Print(L"AllocatPage(%d,%d,%d,%lx)\n", AllocType, MemType, NumPages, Addr);
	efi_status = uefi_call_wrapper(BS->AllocatePages, 4, AllocType, MemType, NumPages, &Addr);
	if (EFI_ERROR(efi_status)) {
		Print(L"Allocate Pages Failed: %d\n", efi_status);
		return efi_status;
	}
	return EFI_SUCCESS;
}
