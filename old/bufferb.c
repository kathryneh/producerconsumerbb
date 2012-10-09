//buffer.c
//#include "buffer.h"
//#include "semaphore.h"
#define BUFFER_SIZE 201;

typedef struct {
  int nextIn;
  int nextOut;
  char bufferMem[201];
  semaphore emptyBuffers;
  //change these to non-pointers 
  semaphore fullBuffers;
} buffer;

void createBuffer(buffer* b){
	(b->nextOut) = 0;
	(b->nextIn) = 0;
	
	//b->emptyBuffers = (semaphore*) malloc(sizeof(semaphore));
	//b->fullBuffers = (semaphore*) malloc(sizeof(semaphore));
	//avoid malloc. 
	createSem(&(b->emptyBuffers), 1);
	createSem(&(b->fullBuffers), 0);
	puts("buffer created");
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



