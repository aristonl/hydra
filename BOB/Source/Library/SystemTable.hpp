#ifndef SystemTable
#include "TableHeaders.hpp"
#include "TextProtocol.hpp"
#include "BootServices.hpp"

struct SystemTable {
  struct TableHeaders Headers;
  unsigned short int* FirmwareVendor;
  unsigned int FirmwareVersion;
  void* ConsoleInputHandle;
  void* ConsoleInput;
  void* ConsoleOutputHandle;
  struct TextProtocol* ConsoleOutput;
  void* StandardErrorHandle, *StandardError;
  void* RuntimeServices;
  struct BootServices* BootServices;
  unsigned long long NumberOfEntries;
  void* ConfigurationTable;
};

#endif