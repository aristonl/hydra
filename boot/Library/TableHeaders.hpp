#ifndef TableHeaders

struct TableHeaders {
  unsigned long long Signature;
  unsigned int Revision, HeaderSize, CRC32, Reserved;
};

#endif