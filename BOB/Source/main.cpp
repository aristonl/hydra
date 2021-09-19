#include "bob.hpp"

void itoa(unsigned long int n, unsigned short int* buffer, int basenumber) {
  unsigned long int hold = n;
  int i=0, j;
  do {
    hold = n % basenumber;
    buffer[i++] = (hold<10) ? (hold+'0') : (hold+'a'-10);
  } while (n /= basenumber);
  buffer[i--] = 0;
  for (j=0;j<i;j++,i--) {
    hold=buffer[j];
    buffer[j] = buffer[i];
    buffer[i] = hold;
  }
}

void* memcpy(void* dst, const void* src, unsigned long long size) {
  for (unsigned long long i=0;i<size;i++) ((unsigned char*) dst)[i] = ((const unsigned char*) src)[i];
  return dst;
}

extern "C" unsigned long long boot(void* ImageHandle, SystemTable* SystemTable) {
  SystemTable->ConOut->Reset(SystemTable->ConOut, 1);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Better Opensource Bootloader (BOB)\r\n");
  // SystemTable->BootServices->Stall(5000000);
  SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, 0);
  unsigned int Status = 0;
  LoadedImageProtocol* LoadedImage;
  Status = SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageProtocolGUID, (void**) &LoadedImage);
  if (Status != 0) SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Error whilst loading FileSystem!\r\n");
  DevicePathProtocol* DevicePath;
  Status = SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &DevicePathProtocolGUID, (void**) &DevicePath);
  if (Status != 0) SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Error whilst loading the device path!\r\n");
  FileSystemProtocol* Volume;
  Status = SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &FileSystemProtocolGUID, (void**) &Volume);
  if (Status != 0) SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Error whilst loading the volume!\r\n");
  FileProtocol* FS;
  Status = Volume->OpenVolume(Volume, &FS);
  if (Status != 0) SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Error whilst loading inferno!\r\n");
  FileProtocol* file = 0;
  Status = FS->Open(FS, &file, (unsigned short*) L"inferno", 0x0000000000000001, 0);
  if (Status != 0) SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Error whilst loading inferno!\r\n");
  void* buffer;
  unsigned long long size = 0x00001000;
  Status = SystemTable->BootServices->AllocatePool(LoaderData, size, (void**) &buffer);
  if (Status != 0) SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"Could not allocate pool!\r\n");
  file->SetPosition(file, 0);
  file->Read(file, &size, buffer);
  unsigned char* inferno = (unsigned char*) buffer;
  for (int i=0;i<5;i++) {
    int c = *inferno;
    unsigned short h[2];
    itoa(c, h, 16);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) h);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L" ");
  }
  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"\n\n\n\r");
  SystemTable->ConOut->OutputString(SystemTable->ConOut, (unsigned short int*) L"This commit is part one of two commits please wait for commit two to run inferno\r\n");
  while(1);
  return 0;
}