#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include <Inferno/Log.h>

/*

#define heapSize 1000000

static char heap[heapSize];
static size_t heapPtr = 0;

void* malloc(size_t size)
{
  // Check if there is enough space on the heap
  if (heapPtr + size > heapSize) {
    // Not enough space on the heap
    return ENOMEM;
	prErr("mm", "malloc(): ENOMEM - Not enough space on the heap.");
  }

  // Allocate memory from the heap
  void* ptr = &heap[heapPtr];
  heapPtr += size;
  return ptr;
}

struct blockMeta {
	size_t size;
	struct blockMeta *next;
	int free;
	int magic; // TODO: For debugging reasons. We can remove this later
};

#define META_SIZE sizeof(struct block_meta)

void *globalBase = NULL;

*/
