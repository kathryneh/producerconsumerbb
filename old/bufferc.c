//buffer.c
//Katie Hawthorne
//COMP 530 HW3
#include "buffer.h"

//#define BUFFER_SIZE 500;

//typedef struct buffer buffer;

//struct buffer{
//  int nextIn;
//  int nextOut;
//  char bufferMem[BUFFER_SIZE];
//  semaphore emptyBuffers;
//  semaphore fullBuffers;
//  buffer* nextBuffer;
//};

void createBuffer(buffer* b, buffer* nextBuffer){
	(b->nextOut) = 0;
	(b->nextIn) = 0;
	createSem(&(b->emptyBuffers), 1);
	createSem(&(b->fullBuffers), 0);
	//I'm onlly mallocing enough for a pointer, so it's not that terrible. 
	(b->nextBuffer) = malloc(sizeof(buffer*));
	(b->nextBuffer) = nextBuffer;
}

void depositBuf(buffer *b, char c){
	down(&(b->emptyBuffers));
	b->bufferMem[b->nextIn] = c;
	b->nextIn = (b->nextIn + 1) % BUFFER_SIZE;
	up(&(b->fullBuffers));
}

char removeBuf(buffer* b){
	char temp = 'a'; 
	down(&(b->fullBuffers));
	temp = b->bufferMem[b->nextOut];
	b->nextOut = (b->nextOut + 1) % BUFFER_SIZE;
	up(&(b->emptyBuffers));
	return temp;
}



