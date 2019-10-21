#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include"mymalloc.h"

Metadata *head = (void*) myblock;
//Initializes the head of the Metadata list
void initialize(){
	head->size = 4096 - sizeof(Metadata);	//The size of Metadata structure is accounted for by subtracting from entire size of myblock of memory
	head->free = 1;				//1 indicating the entire rest of myblock is free
	head->next = NULL;			//NULL as no other Metadata exists
}

//line and function parameters will be used to throw any errors that may occur
void *mymalloc(size_t dataSize, int line, char *file){

	if(dataSize > sizeof(myblock)){		
		printf("Insufficient memory to fulfill malloc request\n filename: %s, Line: %d\n", file, line);
		return NULL;
	}
	if(dataSize == 0){
		return NULL;
	}
	if(!(head->size)){
		initialize();
	}
	
	Metadata *ptr = head;
	Metadata *prevPtr = NULL;
	while(ptr != NULL){	//Runs loop to combine adjacent free blocks for best possible chance to allocate memory
		if(prevPtr != NULL){
			if(prevPtr->free == 1 && ptr->free == 1){
				combineBlocks(prevPtr, ptr);
				ptr = ptr->next;
			}else{
				prevPtr = ptr;
				ptr = ptr->next;
			}
		}else{
			prevPtr = ptr;
			ptr = ptr->next;
		}
	}
	
	prevPtr = NULL;
	ptr = head;

	while(ptr != NULL){
		if(ptr->free == 1 && ptr->size >= (dataSize + sizeof(Metadata))){
			insertMetadata(dataSize, ptr);
			return (void*)(++ptr);
		}else if(ptr->free == 1 && ptr->size == dataSize){
			ptr->free = 0;
			return (void*)(++ptr);
		}
		ptr = ptr->next;
	}
	
	printf("Insufficient memory to fulfill malloc request\nfilename: %s, line: %d\n", file, line);
	return NULL;
}


void combineBlocks(Metadata *prevPtr, Metadata *ptr){
	prevPtr->next = ptr->next;
	prevPtr->size += ptr->size + sizeof(Metadata);
	Metadata *tempPtr = ptr;
	//Need to nullify the struct tempPtr is currently pointing to
	return;
}

//This function will be called everytime a new metadata structure needs to be inserted to bookmark end of requested data
void insertMetadata(size_t dataSize, Metadata *prevMetadata){
	Metadata *bookmark = (Metadata*)((void*)prevMetadata + dataSize + sizeof(Metadata));
	bookmark->size = prevMetadata->size - dataSize - sizeof(Metadata);
	bookmark->free = 1;
	bookmark->next = prevMetadata->next;
	prevMetadata->next = bookmark;
	prevMetadata->size = dataSize;
	prevMetadata->free = 0;
}

//free will set all data back to original null value and change corresponding metadata->free = 1
void myfree(void *ptr, int line, char *file){
	Metadata *prevMetadata = ptr;
	--prevMetadata;


	// Freeing address that are not pointers, (1.a)
	if((char*)ptr < myblock || (char*)ptr >= myblock + 4096){	//(1.b)
		printf("Pointer address is out of bounds\n file: %s, line: %d", file, line);
	}	
	
	int* ptrNum = (int *) (ptr - sizeof(Metadata));
	if(*ptrNum == 0){
		printf("Address is not a pointer\n filename:%s, line: %d\n", file, line);
	}

	if(prevMetadata->free == 1){	//(1.c)
		printf("Redundant freeing: pointer has already been freed\nfilename: %s, line: %d\n", file, line);
	}
	prevMetadata->free = 1;	
	
}
