#include <stdlib.h>

/*
#define ALIGNMENT 8 // must be a power of 2
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define SIZE_T_SIZE (ALIGN(sizeof(size_t))) // header size
// super-naive allocator
void *malloc(size_t size) {
 size_t blk_size = ALIGN(size + SIZE_T_SIZE);
 size_t *header = sbrk(blk_size);
 *header = blk_size | 1; // mark allocated bit
 return (char *)header + SIZE_T_SIZE;
}

void free(void *ptr) {
 size_t *header = (char *)ptr - SIZE_T_SIZE;
 *header = *header & ~1L; // unmark allocated bit
}
*/
