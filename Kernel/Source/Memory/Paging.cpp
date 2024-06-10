#include <Memory/Paging.h>
#include <Memory/Memory.hpp>

// TODO: Use memset and memcpy for bitmap and then use memset for multiple pages


Bitmap Paging::map;

void Paging::FreePage(void* address) {
  // Check if page is already free
  if (Paging::map[(unsigned long long)address/4096]==false) return;
  Paging::map.Set((unsigned long long)address/4096, false);
  Memory::SetFree(Memory::GetFree()+4096);
  Memory::SetUsed(Memory::GetUsed()-4096);
}

void Paging::AllocatePage(void* address) {
  // Check if page is already allocated
  if (Paging::map[(unsigned long long)address/4096]==true) return;
  Paging::map.Set((unsigned long long)address/4096, true);
  Memory::SetFree(Memory::GetFree()-4096);
  Memory::SetUsed(Memory::GetUsed()+4096);
}

void Paging::FreePages(void* address, unsigned long long size) {
  for (unsigned long long x = 0; x < size; x++)
    Paging::FreePage((void*)(unsigned long long)address + (x*4096));
}

void Paging::AllocatePages(void* address, unsigned long long size) {
  for (unsigned long long x = 0; x < size; x++)
    Paging::AllocatePage((void*)(unsigned long long)address + (x*4096));
}

void* Paging::RequestPage() {
  for (unsigned long long x = 0;x<Paging::map.Size*8;x++) {
    if (Paging::map[x]==true) continue;
    Paging::AllocatePage((void*)(x*4096));
    return (void*)(x*4096);
  }

  // TODO: Swap File
  return (void*)0;
}