#include <efi.h>
#include <efilib.h>
#include "drv0.h"


static const EFI_GUID GnuEfiAppsDrv0ProtocolGuid = GNU_EFI_APPS_DRV0_PROTOCOL_GUID;

static struct {
  GNU_EFI_APPS_DRV0_PROTOCOL Proto;
  UINTN Counter;
} InternalGnuEfiAppsDrv0ProtocolData;

static EFI_STATUS EFI_FUNCTION Drv0SayHello(IN const CHAR16 *HelloWho) {
  if (!HelloWho) return EFI_INVALID_PARAMETER;
  Print(L"Hello %s!\n", HelloWho);
  InternalGnuEfiAppsDrv0ProtocolData.Counter++;
  return EFI_SUCCESS;
}

static EFI_STATUS EFI_FUNCTION Drv0GetNumberOfHello(OUT UINTN *NumberOfHello) {
  if (!NumberOfHello) return EFI_INVALID_PARAMETER;
  *NumberOfHello = InternalGnuEfiAppsDrv0ProtocolData.Counter;
  return EFI_SUCCESS;
}

static EFI_STATUS EFI_FUNCTION Drv0Unload(IN EFI_HANDLE ImageHandle) {
  LibUninstallProtocolInterfaces(ImageHandle, &GnuEfiAppsDrv0ProtocolGuid, &InternalGnuEfiAppsDrv0ProtocolData.Proto, NULL);
  Print(L"Driver instance unloaded.\n", ImageHandle);
  return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SysTab) {
  EFI_STATUS Status;
  EFI_LOADED_IMAGE *LoadedImage = NULL;
  InitializeLib(ImageHandle, SysTab);
  InternalGnuEfiAppsDrv0ProtocolData.Proto.SayHello = (GNU_EFI_APPS_DRV0_SAY_HELLO) Drv0SayHello;
  InternalGnuEfiAppsDrv0ProtocolData.Proto.GetNumberOfHello = (GNU_EFI_APPS_DRV0_GET_NUMBER_OF_HELLO) Drv0GetNumberOfHello;
  InternalGnuEfiAppsDrv0ProtocolData.Counter = 0;
  Status = uefi_call_wrapper(BS->OpenProtocol, 6, ImageHandle, &LoadedImageProtocol, (void**)&LoadedImage, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR(Status)) {
    Print(L"Could not open loaded image protocol: %d\n", Status);
    return Status;
  }
  Status = LibInstallProtocolInterfaces(&ImageHandle, &GnuEfiAppsDrv0ProtocolGuid, &InternalGnuEfiAppsDrv0ProtocolData.Proto, NULL);
  if (EFI_ERROR(Status)) {
    Print(L"Error registering driver instance: %d\n", Status);
    return Status;
  }
  LoadedImage->Unload = (EFI_IMAGE_UNLOAD) Drv0Unload;
  Print(L"Driver instance loaded successfully.\n");
  return EFI_SUCCESS;
}
