#ifndef GDT

  namespace IDT {
    typedef struct {
      unsigned short isrLow, cs;
      unsigned char ist, attributes;
      unsigned short isrMid;
      unsigned int isrHigh, null;
    }__attribute__((packed)) Entry;

    typedef struct {
      unsigned short limit;
      unsigned long long base;
    }__attribute__((packed)) Table;
  }

#endif