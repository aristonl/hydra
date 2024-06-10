#pragma once
#include <Memory/Memory.hpp>

namespace Paging {
  extern Bitmap map;
  void FreePage(void* address);
  void AllocatePage(void* address);
  void FreePages(void* address, unsigned long long size);
  void AllocatePages(void* address, unsigned long long size);
  void* RequestPage();
}