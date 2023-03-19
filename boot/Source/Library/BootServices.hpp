#ifndef BootServices
#include "Memory.hpp"
#include "GUID.hpp"

struct BootServices {
  TableHeaders Headers;
  void* RaiseTPL, *RestoreTPL;
  unsigned long long (*AllocatePages)(AllocationType Type, unsigned long long MemoryType, unsigned long long Pages, unsigned long long* Memory);
  void* FreePages;
  unsigned long long (*GetMemoryMap)(unsigned long long* MemoryMapSize, MemoryDescriptor* MemoryMap, unsigned long long* MapKey, unsigned long long* DescriptorSize, unsigned int* DescriptorVersion);
  unsigned long long (*AllocatePool)(unsigned long long PoolType, unsigned long long Size, void** Buffer);
  void* FreePool, *CreateEvent, *SetTimer, *WaitForEvent, *SignalEvent, *CloseEvent, *CheckEvent, *InstallProtocolInterface, *ReinstallProtocolInterface, *UninstallProtocolInterface;
  unsigned long long (*HandleProtocol)(void* Handle, struct GUID* Protocol, void** Interface);
  void* Reserved, *RegisterProtocolNotify, *LocateHandle, *LocateDevicePath, *InstallConfigurationTable, *LoadImage, *StartImage, *Exit, *UnloadImage;
  unsigned long long (*ExitBootServices)(void* ImageHandle, unsigned long long MapKey);
  void* GetNextMonotonicCount, *Stall;
  unsigned long long (*SetWatchdogTimer)(unsigned long long Timeout, unsigned long long WatchdogCode, unsigned long long DataSize, unsigned short int* WatchdogData);
  void* ConnectController, *DisconnectController, *OpenProtocol, *CloseProtocol, *OpenProtocolInformation, *ProtocolsPerHandle, *LocateHandleBuffer;
  unsigned long long (*LocateProtocol)(struct GUID* Protocol, void* Registration, void** Interface);
  void* InstallMultipleProtocolInterfaces, *UninstallMultipleProtocolInterfaces, *CalculateCrc32, *CopyMem, *SetMem, *CreateEventEx;
};

#endif