#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>

#ifndef _mymalloc_h_
#define _mymalloc_h_

#define malloc( x ) mymalloc(x, __LINE__, __FILE__)
#define free( x ) myfree(x, __LINE__, __FILE__)

static char myblock[4096];

//Manages the information pertaining to the chunk of data still existing and being used (almost like a bookmark)
typedef struct Metadata{	//Metadata struct 
	int free;	//Indicator of if chunk of data is free or not
	size_t size;	//size of data of the chunk
	struct Metadata *next;	//Pointer to the following Metadata
}Metadata;

void initialize();
void* mymalloc(size_t, int, char*);
void combineBlocks(Metadata *prevPtr, Metadata *ptr);
void insertMetadata(size_t dataSize, Metadata *prevMetadata);
void myfree(void *ptr, int line, char *file);

#endif
