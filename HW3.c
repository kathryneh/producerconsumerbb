/*HW3.c
 *COMP 530
 *Katie Hawthorne
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "st.h"
#include "semaphore.h"
#include "buffer.c"

#define LINE_LENGTH 80

void* produceChar(void *s);
void* consumeStar(void *s);
void* consumeNewLine(void *s);
void* countChars(void *s);

int main(int argc, char const *argv[]) {

	st_init();
	buffer b; // b = the first buffer between input and starchecking
	buffer c; // c = the second buffer between starchecking and newline checking
	buffer d; // d = the third buffer that will lead to outputting lines 
	
	//creating the buffers; each buffer has its own buffer and a pointer to the next one
	createBuffer(&b, &c);
	createBuffer(&c, &d);
	createBuffer(&d, NULL);

	//declaring the buffer aliases.
	//the producer and the starBuffer point to the same buffer
	buffer* producer = &b;
	buffer* starBuffer = &b;
	buffer* newLineBuffer = &c;
	buffer* countingBuffer = &d;

//	create the producer thread
	if (st_thread_create(produceChar, producer, 0, 0) == NULL) {
		perror("st_thread_create for producer thread failure");
		exit(1);
	}
	
//	create the star consumer thread
	if (st_thread_create(consumeStar, starBuffer, 0, 0) == NULL) {
		perror("st_thread_create for consumer thread failure");
		exit(1);
	}

//	create the new line thread
	if (st_thread_create(consumeNewLine, newLineBuffer, 0, 0) == NULL) {
		perror("st_thread_create for consumer thread failure");
		exit(1);
	}

//	create the counting and output thread
	if (st_thread_create(countChars, countingBuffer, 0, 0) == NULL) {
		perror("st_thread_create for consumer thread failure");
		exit(1);
	}
	//kill the parent. 
	st_thread_exit(NULL);
	return 0;
}

/* produceChar takes in a pointer to a buffer
 * and takes input characters from a file and adds them to the first buffer
 */
void* produceChar(void *producer){
	char curChar;
	while ((curChar = getchar()) != EOF){
		depositBuf(producer, curChar);
	}
	depositBuf(producer, curChar);
	st_thread_exit(NULL);
}
/*consumeStar takes a pointer to a buffer as the parameter
 * and grabs chars from the buffer that were deposited by the producer
 * it tests to see if there are multiple stars and will replace them 
 * with a carrot if two are consecutive.
 */
void* consumeStar(void *starBuf){
	char temp;
	buffer* starBuffer = starBuf;
	buffer *newLineBuffer = starBuffer->nextBuffer;
	while((temp = removeBuf(starBuffer)) != EOF){
		//if the first char is not a star, just put it in the next buffer
		if (temp == '*'){
			char temp2;
			//but if it is a star, check the next char
			if ((temp2 = removeBuf(starBuffer)) != EOF){
				//if also a char, put in a carrot
				if (temp2 == '*'){
					depositBuf(newLineBuffer, '^');
				}	
				//else just add both chars to the next buffer. 
				else{
					depositBuf(newLineBuffer, temp);
					depositBuf(newLineBuffer, temp2);		
				}
			}
		}
		else{//normal character, send it along
			depositBuf(newLineBuffer, temp);
		}
	}
	//need to kill by passing along EOF
	depositBuf(newLineBuffer, temp);
	st_thread_exit(NULL);
}
/*
 *consumeNewLine takes a buffer as a parameter
 *and takes input from that buffer; it checks for new lines and replaces 
 *as spaces.
 *It then outputs to the next buffer, the counting buffer, for the end of processing. 
 */
void* consumeNewLine(void *newLineBuf){
	char temp;
	buffer* newLineBuffer = newLineBuf;
	buffer *countingBuffer = newLineBuffer->nextBuffer;
	while((temp = removeBuf(newLineBuffer)) != EOF){
		//if we find a new line, replace it with a space. 
		if (temp == '\n'){
			depositBuf(countingBuffer, ' ');
		}
		//just send along the next char
		else{
			depositBuf(countingBuffer, temp);
		}
	}
	//send along the EOF
	depositBuf(countingBuffer, temp);
	st_thread_exit(NULL);
}
/* countChars takes in a pointer to a buffer and outputs to stdout using printf. 
 * It outputs only 80 character lines and basically just counts to make sure that it
 * has the right number of characters. 
 */
void* countChars(void *countingBuf){
	char temp;
	//declare an array to hold the chars to output
	char charOutput[LINE_LENGTH];
	int count = 0;
	int lineLength = LINE_LENGTH;//see? I'm using defined constants. 
	buffer* countingBuffer = countingBuf;
	while((temp = removeBuf(countingBuffer)) != EOF){
		charOutput[count] = temp;
		//if the count is equal to the maximum size we're outputting...
		if (count == LINE_LENGTH - 1){
			//print it out, restart counting.
			//and only prints if the line is full.  
			printf("%.80s\n", charOutput);
			count = 0;
		}
		else{
		//just keep counting, just keep counting...
			count++;
		}
	}
	//exit at EOF
	st_thread_exit(NULL);
}


