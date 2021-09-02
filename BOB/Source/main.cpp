#define UNICODE
struct Input;
typedef struct InputKey {
	unsigned short ScanCode;
	unsigned short UnicodeChar;
} InputKey;
typedef void(*EventNotify)(void* Event, void* Context);
typedef unsigned long long (*ResetInput)(struct Input* This, unsigned char ExtendedVerification);
typedef unsigned long long (*ReadInput)(struct Input* This, InputKey* Key);
typedef struct Input {
	ResetInput Reset;
	ReadInput Read;
	void* WaitForKey;
} Input;
struct Output;
typedef unsigned long long(*ResetOutput) (struct Output* This, unsigned char ExtendedVerification);
typedef unsigned long long(*PrintOutput) (struct Output* This, unsigned short int* String);
typedef unsigned long long(*TestOutput) (struct Output* This, unsigned short int* String);
typedef unsigned long long(*QueryOutput) (struct Output* This, unsigned long long ModeNumber, unsigned long long* Columns, unsigned long long* Rows);
typedef unsigned long long(*SetOutput) (struct Output* This, unsigned long long ModeNumber);
typedef unsigned long long(*Font) (struct Output* This, unsigned long long Attribute);
typedef struct Output {
  ResetOutput Reset;
  PrintOutput Print;
  TestOutput TestString;
  QueryOutput QueryMode;
  SetOutput SetMode;
  Font SetFont;
} Output;
typedef struct TableHeaders {
  unsigned long long Signature;
  unsigned int Revision;
  unsigned int HeaderSize;
  unsigned int CRC32;
  unsigned int Reserved;
} TableHeaders;
typedef struct SystemTable {
  TableHeaders hrd;
  unsigned short int *FirmwareVendor;
  unsigned int FirmwareVersion;
  void* ConsoleInHandle;
  Input* ConIn;
  void* ConsoleOutHandle;
  Output* ConOut;
} SystemTable;

extern "C" unsigned long long boot(void* ImageHandle, SystemTable* systemTable) {
  systemTable->ConOut->Reset(systemTable->ConOut, 1);
  systemTable->ConOut->Print(systemTable->ConOut, (unsigned short int*) L"BOB\r\n");
  while(1);
  return 0;
}