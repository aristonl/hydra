#ifndef GOPModeInfo

struct GOPModeInfo {
  unsigned int Version, Width, Height;
  void* PixelFormat, *PixelInformation;
  unsigned int PPSL;
};

#endif

#ifndef GOPProtocolMode

struct GOPProtocolMode {
  unsigned int MaxMode, CurrentMode;
  GOPModeInfo* ModeInfo;
  unsigned long long SizeofModeInfo, Address, Size;
};

#endif

#ifndef GOP

struct GOP {
  void* QueryMode, *SetMode, *BLT;
  GOPProtocolMode* Mode;
};

#endif