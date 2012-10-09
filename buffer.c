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

/* createBuffer just defines all of the things needed in the buffer struct
 * and also acts like a linked list in that each buffer will have a link to
 * the next buffer that it will be depositing into. 
 */
void createBuffer(buffer* b, buffer* nextBuffer){
	(b->nextOut) = 0;
	(b->nextIn) = 0;
	createSem(&(b->emptyBuffers), 1);
	createSem(&(b->fullBuffers), 0);
	//I'm onlly mallocing enough for a pointer, so it's not that terrible. 
	(b->nextBuffer) = malloc(sizeof(buffer*));
	(b->nextBuffer) = nextBuffer;
}

/*depositBuffer locks the semaphore upon entry, takes in a character, and adds
 * the character to the array of the buffer contents. 
 * it then releases the semaphore upon exit
 * Importantly, it does a mod, so each buffer has a circular buffer and we
 * won't get memory overflow. 
 */
void depositBuf(buffer *b, char c){
	down(&(b->emptyBuffers));//lock 
	b->bufferMem[b->nextIn] = c;
	b->nextIn = (b->nextIn + 1) % BUFFER_SIZE;
	up(&(b->fullBuffers));//unlock
}

/*removeBuffer takes in a pointer to a buffer and just removes the next value
 * from the buffer to do processing upon.
 */
char removeBuf(buffer* b){
	char temp = 'a'; 
	down(&(b->fullBuffers));//lock
	temp = b->bufferMem[b->nextOut];
	b->nextOut = (b->nextOut + 1) % BUFFER_SIZE;
	up(&(b->emptyBuffers));//unlock
	return temp;
}



