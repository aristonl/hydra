#ifndef BOB
#define BOB

#define UNICODE

typedef struct GUID {
  unsigned int Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char Data4[8];
} GUID;

struct GUID GOPGUID = {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
struct GUID LoadedImageProtocolGUID = {0x5b1b31a1,  0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
struct GUID FileSystemProtocolGUID = {0x0964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
struct GUID DevicePathProtocolGUID = {0x09576e91, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
struct GUID FileInfoGUID = {0x9576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

struct TextProtocol;
struct InputProtocol;
struct BootServices;
struct GOP;
struct FileSystemProtocol;
struct FileProtocol;
struct FileSystemProtocol;

typedef enum {
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef struct Time {
	unsigned short Year;
	unsigned char Month;
	unsigned char Day;
	unsigned char Hour;
	unsigned char Minute;
	unsigned char Second;
	unsigned char Pad1;
	unsigned int Nanosecond;
	unsigned short TimeZone;
	unsigned char DayLight;
	unsigned char Pad2;
} Time;

typedef struct TimeCapabilities {
	unsigned int Resolution;
	unsigned int Accuracy;
	unsigned char SetsToZero;
} TimeCapabilities;

typedef enum ResetType { ResetCold, ResetWarm, ResetShutdown, ResetPlatformSpecific } ResetType;

typedef struct ConfigurationTable {
  GUID VendorGuid;
  void* VendorTable;
} ConfigurationTable;

typedef struct TableHeaders {
  unsigned long long Signature;
  unsigned int Revision;
  unsigned int HeaderSize;
  unsigned int CRC32;
  unsigned int Reserved;
} TableHeaders;

typedef enum AllocateType { AllocateAnyPages, AllocateMaxAddress, AllocateAddress, MaxAllocateType } AllocateType;
typedef enum TimerDelay { TimerCancel, TimerPeriodic, TimerRelative } TimerDelay;

typedef struct MemoryDescriptor {
  unsigned int Type;
  unsigned long long PhysicalStart;
  unsigned long long VirtualStart;
  unsigned long long NumberOfPages;
  unsigned long long Attribute;
} MemoryDescriptor;

typedef enum InterfaceType { NativeInterface } InterfaceType;
typedef enum LocateSearchType { AllHandles, ByRegisterNotify, ByProtocol } LocateSearchType;

typedef struct OpenProtocolInformationEntry {
  void* AgentHandle;
  void* ControllerHandle;
  unsigned int Attributes;
  unsigned int OpenCount;
} OpenProtocolInformationEntry;

typedef struct DevicePathProtocol {
  unsigned char Type;
  unsigned char SubType;
  unsigned char Length[2];
} DevicePathProtocol;

typedef struct CapsuleHeaders {
  GUID CapsuleGuid;
  unsigned int HeaderSize;
  unsigned int Flags;
  unsigned int CapsuleImageSize;
} CapsuleHeaders;

typedef struct InputKey {
	unsigned short ScanCode;
	unsigned short UnicodeChar;
} InputKey;

typedef void(*EventNotify)(void* Event, void* Context);
typedef unsigned long long (*InputReset)(struct InputProtocol* This, unsigned char ExtendedVerification);
typedef unsigned long long (*ReadKey)(struct InputProtocol* This, InputKey* Key);

typedef struct InputProtocol {
	InputReset Reset;
	ReadKey ReadKeyStroke;
	void * WaitForKey;
} InputProtocol;

typedef struct TextMode {
  int MaxMode;
  int Mode;
  int Attribute;
  int CursorColumn;
  int CursorRow;
  unsigned char CursorVisible;
} TextMode;

typedef unsigned long long (*TextReset)(struct TextProtocol* This, unsigned char ExtendedVerification);
typedef unsigned long long (*OutputString)(struct TextProtocol* This, unsigned short int* String);
typedef unsigned long long (*TestString)(struct TextProtocol* This, unsigned short int* String);
typedef unsigned long long (*QueryMode)(struct TextProtocol* This, unsigned long long ModeNumber, unsigned long long* Columns, unsigned long long* Rows);
typedef unsigned long long (*SetMode)(struct TextProtocol* This, unsigned long long ModeNumber);
typedef unsigned long long (*SetAttribute)(struct TextProtocol* This, unsigned long long Attribute);
typedef unsigned long long (*ClearScreen)(struct TextProtocol* This);
typedef unsigned long long (*SetCursorPosition)(struct TextProtocol* This, unsigned long long Column, unsigned long long Row);
typedef unsigned long long (*EnableCursor)(struct TextProtocol* This, unsigned char Visible);

typedef struct TextProtocol {
  TextReset Reset;
  OutputString OutputString;
  TestString TestString;
  QueryMode QueryMode;
  SetMode SetMode;
  SetAttribute SetAttribute;
  ClearScreen ClearScreen;
  SetCursorPosition SetCursorPosition;
  EnableCursor EnableCursor;
  TextMode* Mode;
} TextProtocol;

typedef unsigned long long (*RaiseTPL)(unsigned long long NewTpl);
typedef unsigned long long (*RestoreTPL)(unsigned long long OldTpl);
typedef unsigned long long (*AllocatePages)(AllocateType Type, unsigned long long MemoryType, unsigned long long Pages, unsigned long long* Memory);
typedef unsigned long long (*FreePages)(unsigned long long Memory, unsigned long long Pages);
typedef unsigned long long (*GetMemoryMap)(unsigned long long* MemoryMapSize, MemoryDescriptor* MemoryMap, unsigned long long* MapKey, unsigned long long* DescriptorSize, unsigned int* DescriptorVersion);
typedef unsigned long long (*AllocatePool)(unsigned long long PoolType, unsigned long long Size, void** Buffer);
typedef unsigned long long (*FreePool)(void* Buffer);
typedef unsigned long long (*CreateEvent)(unsigned int Type, unsigned long long NotifyTpl, EventNotify NotifyFunction, void* NotifyContext, GUID* EventGroup, void ** Event);
typedef unsigned long long (*SetTimer)(void* Event, TimerDelay Type, unsigned long long TriggerTime);
typedef unsigned long long (*WaitForEvent)(unsigned long long NumberOfEvents, void ** Event, unsigned long long* Index);
typedef unsigned long long (*SignalEvent)(void* Event);
typedef unsigned long long (*CloseEvent)(void* Event);
typedef unsigned long long (*CheckEvent)(void* Event);
typedef unsigned long long (*InstallProtocolInterface)(void** Handle, GUID* Protocol, InterfaceType InterfaceType, void* Interface);
typedef unsigned long long (*ReinstallProtocolInterface)(void* Handle, GUID* Protocol, void* OldInterface, void* NewInterface);
typedef unsigned long long (*UninstallProtocolInterface)(void* Handle, GUID* Protocol, void* Interface);
typedef unsigned long long (*HandleProtocol)(void* Handle, GUID* Protocol, void** Interface);
typedef unsigned long long (*RegisterProtocolNotify)(GUID* Protocol, void * Event, void** Registration);
typedef unsigned long long (*LocateHandle)(LocateSearchType SearchType, GUID* Protocol, void* SearchKey, unsigned long long* BufferSize, void** Buffer);
typedef unsigned long long (*LocateDevicePath)(GUID* Protocol, DevicePathProtocol** DevicePath, void** Device);
typedef unsigned long long (*InstallConfigurationTable)(GUID* Guid, void* Table);
typedef unsigned long long (*LoadImage)(unsigned char BootPolicy, void* ParentImageHandle, DevicePathProtocol* DevicePath, void* SourceBuffer, unsigned long long SourceSize, void** ImageHandle);
typedef unsigned long long (*StartImage)(void* ImageHandle, unsigned long long* ExitDataSize, unsigned short int** ExitData);
typedef unsigned long long (*Exit)(void* ImageHandle, unsigned long long ExitStatus, unsigned long long ExitDataSize, unsigned short int* ExitData);
typedef unsigned long long (*UnloadImage)(void* ImageHandle);
typedef unsigned long long (*ExitBootServices)(void* ImageHandle, unsigned long long MapKey);
typedef unsigned long long (*GetNextMonotonicCount)(unsigned long long* Count);
typedef unsigned long long (*Stall)(unsigned long long Microseconds);
typedef unsigned long long (*SetWatchdogTimer)(unsigned long long Timeout, unsigned long long WatchdogCode, unsigned long long DataSize, unsigned short int* WatchdogData);
typedef unsigned long long (*ConnectController)(void* ControllerHandle, void** DriverImageHandle, DevicePathProtocol* RemainingDevicePath, unsigned char Recursive);
typedef unsigned long long (*DisconnectController)(void* ControllerHandle, void* DriverImageHandle, void* ChildHandle);
typedef unsigned long long (*OpenProtocol)(void* Handle, GUID* Protocol, void** Interface, void* AgentHandle, void* ControllerHandle, unsigned int Attributes);
typedef unsigned long long (*CloseProtocol)(void* Handle, GUID* Protocol, void* AgentHandle, void* ControllerHandle);
typedef unsigned long long (*OpenProtocolInformation)(void* Handle, GUID* Protocol, OpenProtocolInformationEntry** EntryBuffer, unsigned long long* EntryCount);
typedef unsigned long long (*ProtocolsPerHandle)(void* Handle, GUID*** ProtocolBuffer, unsigned long long* ProtocolBufferCount);
typedef unsigned long long (*LocateHandleBuffer)(LocateSearchType SearchType, GUID* Protocol, void* SearchKey, unsigned long long* NoHandles, void*** Buffer);
typedef unsigned long long (*LocateProtocol)(GUID* Protocol, void* Registration, void** Interface);
typedef unsigned long long (*InstallMultipleProtocolInterfaces)(void** Handle, ...);
typedef unsigned long long (*UninstallMultipleProtocolInterfaces)(void** Handle, ...);
typedef unsigned long long (*CalculateCrc32)(void* Data, unsigned long long DataSize, unsigned int* Crc32);
typedef unsigned long long (*CopyMem)(void* Destination, void* Source, unsigned long long Length);
typedef unsigned long long (*SetMem)(void* Buffer, unsigned long long Size, unsigned char Value);
typedef unsigned long long (*CreateEventEx)(unsigned int Type, unsigned long long NotifyTpl, EventNotify NotifyFunction, void* NotifyContext, GUID* EventGroup, void ** Event);

typedef struct BootServices {
  TableHeaders Hdr;
  RaiseTPL RaiseTPL;
  RestoreTPL RestoreTPL;
  AllocatePages AllocatePages;
  FreePages FreePages;
  GetMemoryMap GetMemoryMap;
  AllocatePool AllocatePool;
  FreePool FreePool;
  CreateEvent CreateEvent;
  SetTimer SetTimer;
  WaitForEvent WaitForEvent;
  SignalEvent SignalEvent;
  CloseEvent CloseEvent;
  CheckEvent CheckEvent;
  InstallProtocolInterface InstallProtocolInterface;
  ReinstallProtocolInterface ReinstallProtocolInterface;
  UninstallProtocolInterface UninstallProtocolInterface;
  HandleProtocol HandleProtocol;
  void* Reserved;
  RegisterProtocolNotify RegisterProtocolNotify;
  LocateHandle LocateHandle;
  LocateDevicePath LocateDevicePath;
  InstallConfigurationTable InstallConfigurationTable;
  LoadImage LoadImage;
  StartImage StartImage;
  Exit Exit;
  UnloadImage UnloadImage;
  ExitBootServices ExitBootServices;
  GetNextMonotonicCount GetNextMonotonicCount;
  Stall Stall;
  SetWatchdogTimer SetWatchdogTimer;
  ConnectController ConnectController;
  DisconnectController DisconnectController;
  OpenProtocol OpenProtocol;
  CloseProtocol CloseProtocol;
  OpenProtocolInformation OpenProtocolInformation;
  ProtocolsPerHandle ProtocolsPerHandle;
  LocateHandleBuffer LocateHandleBuffer;
  LocateProtocol LocateProtocol;
  InstallMultipleProtocolInterfaces InstallMultipleProtocolInterfaces;
  UninstallMultipleProtocolInterfaces UninstallMultipleProtocolInterfaces;
  CalculateCrc32 CalculateCrc32;
  CopyMem CopyMem;
  SetMem SetMem;
  CreateEventEx CreateEventEx;
} BootServices;

typedef unsigned long long (*GetTime)(Time* Time, TimeCapabilities* Capabilities);
typedef unsigned long long (*SetTime)(Time* Time);
typedef unsigned long long (*GetWakeupTime)(unsigned char* Enabled, unsigned char* Pending, Time* Time);
typedef unsigned long long (*SetWakeupTime)(unsigned char Enable, Time* Time);
typedef unsigned long long (*SetVirtualAddressMap)(unsigned long long MemoryMapSize, unsigned long long DescriptorSize, unsigned int DescriptorVersion, MemoryDescriptor* VirtualMap);
typedef unsigned long long (*ConvertPointer)(unsigned long long DebugDisposition, void** Address);
typedef unsigned long long (*GetVariable)(unsigned short int* VariableName, GUID* VendorGuid, unsigned int* Attributes, unsigned long long* DataSize, void* Data);
typedef unsigned long long (*GetNextVariableName)(unsigned long long* VariableNameSize, unsigned short int* VariableName, GUID* VendorGuid);
typedef unsigned long long (*SetVariable)(unsigned short int* VariableName, GUID* VendorGuid, unsigned int Attributes, unsigned long long DataSize, void* Data);
typedef unsigned long long (*GetNextHighMonoCount)(unsigned int* HighCount);
typedef unsigned long long (*ResetSystem)(ResetType ResetType, unsigned long long ResetStatus, unsigned long long DataSize, void* ResetData);
typedef unsigned long long (*UpdateCapsule)(CapsuleHeaders** CapsuleHeaderArray, unsigned long long CapsuleCount, unsigned long long ScatterGatherList);
typedef unsigned long long (*QueryCapsuleCapabilities)(CapsuleHeaders** CapsuleHeaderArray, unsigned long long CapsuleCount, unsigned long long* MaximumCapsuleSize, ResetType* ResetType);
typedef unsigned long long (*QueryVariableInfo)(unsigned int Attributes, unsigned long long* MaximumVariableStorageSize, unsigned long long* RemainingVariableStorageSize, unsigned long long* MaximumVariableSize);

typedef struct RuntimeServices {
  TableHeaders Hdr;
  GetTime GetTime;
  SetTime SetTime;
  GetWakeupTime GetWakeupTime;
  SetWakeupTime SetWakeupTime;
  SetVirtualAddressMap SetVirtualAddressMap;
  ConvertPointer ConvertPointer;
  GetVariable GetVariable;
  GetNextVariableName GetNextVariableName;
  SetVariable SetVariable;
  GetNextHighMonoCount GetNextHighMonotonicCount;
  ResetSystem ResetSystem;
  UpdateCapsule UpdateCapsule;
  QueryCapsuleCapabilities QueryCapsuleCapabilities;
  QueryVariableInfo QueryVariableInfo;
} RuntimeServices;

typedef struct FileToToken {
  void* Event;
  unsigned long long Status;
  unsigned long long BufferSize;
  void* Buffer;
} FileToToken;

typedef unsigned long long (*FileOpen)(struct FileProtocol *This, struct FileProtocol **NewHandle, unsigned short int *FileName, unsigned long long OpenMode, unsigned long long Attributes);
typedef unsigned long long (*FileClose)(struct FileProtocol *This);
typedef unsigned long long (*FileDelete)(struct FileProtocol *This);
typedef unsigned long long (*FileRead)(struct FileProtocol *This, unsigned long long *BufferSize, void *Buffer);
typedef unsigned long long (*FileWrite)(struct FileProtocol *This, unsigned long long *BufferSize, void *Buffer);
typedef unsigned long long (*FileGetPosition)(struct FileProtocol *This, unsigned long long *Position);
typedef unsigned long long (*FileSetPosition)(struct FileProtocol *This, unsigned long long Position);
typedef unsigned long long (*FileGetInfo)(struct FileProtocol *This, GUID* InformationType, unsigned long long BufferSize, void* Buffer);
typedef unsigned long long (*FileSetInfo)(struct FileProtocol *This, GUID* InformationType, unsigned long long BufferSize, void* Buffer);
typedef unsigned long long (*FileFlush)(struct FileProtocol *This);
typedef unsigned long long (*FileOpenEx)(struct FileProtocol *This, struct FileProtocol** NewHandle, unsigned short int* FileName, unsigned long long OpenMode, unsigned long long Attributes, FileToToken* Token);
typedef unsigned long long (*FileReadEx)(struct FileProtocol *This, FileToToken* Token);
typedef unsigned long long (*FileWriteEx)(struct FileProtocol *This, FileToToken* Token);
typedef unsigned long long (*FileFlushEx)(struct FileProtocol *This, FileToToken* Token);

typedef struct FileInfo {
  unsigned long long Size;
  unsigned long long FileSize;
  unsigned long long PhysicalSize;
  Time CreateTime;
  Time LastAccessTime;
  Time ModificationTime;
  unsigned long long Attribute;
  unsigned short int FileName[];
} FileInfo;

typedef struct FileProtocol {
  unsigned long long Revision;
  FileOpen Open;
  FileClose Close;
  FileDelete Delete;
  FileRead Read;
  FileWrite Write;
  FileGetPosition GetPosition;
  FileSetPosition SetPosition;
  FileGetInfo GetInfo;
  FileSetInfo SetInfo;
  FileFlush Flush;
  FileOpenEx OpenEx;
  FileReadEx ReadEx;
  FileWriteEx WriteEx;
  FileFlushEx FlushEx;
} FileProtocol;

typedef unsigned long long (*FileSystemProtocolOpenVolume)(struct FileSystemProtocol* This, FileProtocol** Root);

typedef struct FileSystemProtocol {
  unsigned long long Revision;
  FileSystemProtocolOpenVolume OpenVolume;
} FileSystemProtocol;

typedef struct SystemTable {
	TableHeaders hdr;
	unsigned short int* FirmwareVendor;
	unsigned int FirmwareVersion;
	void* ConsoleInHandle;
	InputProtocol* ConIn;
	void* ConsoleOutHandle;
	TextProtocol* ConOut;
	void* StandardErrorHandle;
	TextProtocol* StdErr;
	RuntimeServices* RuntimeServices;
	BootServices* BootServices;
	unsigned long long NumberOfTableEntries;
	ConfigurationTable* ConfigurationTable;
} SystemTable;

typedef struct LoadedImageProtocol {
  unsigned int Revision;
  void* ParentHandle;
  SystemTable* SystemTable;
  void* DeviceHandle;
  DevicePathProtocol* FilePath;
  void* Reserved;
  unsigned int LoadOptionsSize;
  void* LoadOptions;
  void* ImageBase;
  unsigned long long ImageSize;
} LoadedImageProtocol;

typedef enum GOPBLTOP { BLTVideoFill, BLTVideo2BLTBuffer, BLTBuffer2Video, BLTV2V, GOPBLTOPMax } GOPBLTOP;

typedef struct GOPBLTPX {
  unsigned char Blue;
  unsigned char Green;
  unsigned char Red;
  unsigned char Reserved;
} GOPBLTPX;

typedef enum GOPPXFormat { PixelRedGreenBlueReserved8BitPerColor, PixelBlueGreenRedReserved8BitPerColor, PixelBitMask, PixelBltOnly, PixelFormatMax } GOPPXFormat;

typedef struct PXBM {
  unsigned int RedMask;
  unsigned int GreenMask;
  unsigned int BlueMask;
  unsigned int ReservedMask;
} PXBM;

typedef struct GOPModeInfo {
  unsigned int Version;
  unsigned int HorizontalResolution;
  unsigned int VerticalResolution;
  GOPPXFormat PixelFormat;
  PXBM PixelInformation;
  unsigned int PixelsPerScanLine;
} GOPModeInfo;

typedef struct GOPProtocolMode {
  unsigned int MaxMode;
  unsigned int Mode;
  GOPModeInfo* Info;
  unsigned long long SizeOfInfo;
  unsigned long long FrameBufferBase;
  unsigned long long FrameBufferSize;
} GOPProtocolMode;

typedef unsigned long long (*GOPQueryMode)(struct GOP* This, unsigned int ModeNumber, unsigned long long* SizeOfInfo, GOPModeInfo** Info);
typedef unsigned long long (*GOPSetMode)(struct GOP* This, unsigned int ModeNumber);
typedef unsigned long long (*GOPBLT)(struct GOP* This, GOPBLTPX* BltBuffer, GOPBLTOP BltOperation, unsigned long long SourceX, unsigned long long SourceY, unsigned long long DestinationX, unsigned long long DestinationY, unsigned long long Width, unsigned long long Height, unsigned long long Delta);

typedef struct GOP {
  GOPQueryMode QueryMode;
  GOPSetMode SetMode;
  GOPBLT Blt;
  GOPProtocolMode* Mode;
} GOP;

#endif