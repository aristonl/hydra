#ifndef DevicePathProtocol

struct DevicePathProtocol {
  unsigned char Type, SubType, Length[2];
};

#endif

#ifndef LoadedImageProtocol

struct LoadedImageProtocol {
  unsigned int Revision;
  void* ParentHandle;
  SystemTable* SystemTable;
  void* DeviceHandle;
  DevicePathProtocol* FilePath;
  void* Reserved;
  unsigned int LoadOptionsSize;
  void* LoadOptions, *ImageBase;
  unsigned long long ImageSize;
};

#endif

#ifndef FileInfo

struct FileInfo {
  unsigned long long Size, FileSize, PhysicalSize;
  void* CreateTime, *LastAccessTime, *ModificationTime;
  unsigned long long Attribute;
  unsigned short int FileName[];
};

#endif

#ifndef FileProtocol

struct FileProtocol {
  unsigned long long Revision;
  unsigned long long (*Open)(struct FileProtocol *This, struct FileProtocol **NewHandle, unsigned short int *FileName, unsigned long long OpenMode, unsigned long long Attributes);
  unsigned long long (*Close)(struct FileProtocol *This);
  void* Delete;
  unsigned long long (*Read)(struct FileProtocol *This, unsigned long long *BufferSize, void *Buffer);
  void* Write;
  unsigned long long (*GetPosition)(struct FileProtocol *This, unsigned long long *Position);
  unsigned long long (*SetPosition)(struct FileProtocol *This, unsigned long long Position);
  unsigned long long (*GetInfo)(struct FileProtocol *This, GUID* InformationType, unsigned long long BufferSize, void* Buffer);
  void* SetInfo, *Flush, *OpenEx, *ReadEx, *WriteEx, *FlushEx;
};

#endif

#ifndef FileSystemProtocol

struct FileSystemProtocol {
  unsigned long long Revision;
  unsigned long long (*OpenVolume)(struct FileSystemProtocol* This, FileProtocol** Root);
};

#endif