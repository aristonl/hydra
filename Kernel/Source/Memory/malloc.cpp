#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>


/*
 *	malloc()
 */ 
void *malloc(size_t size) {
	void *p = sbrk(0);
	void *request = sbrk(size);
	
	if (request == (void*) -1) {
		return NULL;
	} else {
		return p;
	}
}

struct blockMeta {
	size_t size;
	struct blockMeta *next;
	int free;
	int magic; // TODO: For debugging reasons. We can remove this later
}

#define META_SIZE sizeof(struct block_meta)

void *globalBase = NULL;


