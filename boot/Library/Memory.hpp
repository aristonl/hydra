#ifndef AllocationType
enum AllocationType { AllocateAnyPages, AllocateMaxAddress, AllocateAddress, MaxAllocateType };
#endif

#ifndef MemoryDescriptor

struct MemoryDescriptor {
  unsigned int Type;
  unsigned long long PhysicalStart, VirtualStart, NumberOfPages, Attribute;
};

#endif

#ifndef memcmp

int memcmp(const void* aptr, const void* bptr, unsigned long long n) {
	const unsigned char* a = (unsigned char*) aptr, *b = (unsigned char*) bptr;
	for (unsigned long long i = 0; i < n; i++) {
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

#endif