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
#include "bufferc.c"

#define DEFAULT_OUT stdout
#define LINE_LENGTH 80

void* produceChar(void *s);
void* consumeStar(void *s);
void* consumeNewLine(void *s);
void* countChars(void *s);

int main(int argc, char const *argv[]) {

	st_init();
	FILE *output = DEFAULT_OUT;
	buffer b; // b = the first buffer between input and starchecking
	buffer c; // c = the second buffer between starchecking and newline checking
	buffer d; // d = the third buffer that will lead to outputting lines 
	createBuffer(&b, &c);
	createBuffer(&c, &d);
	createBuffer(&d, NULL);

	buffer* starConsumer = &b;
	buffer* producer = &b;

	buffer* newLineBuffer = &c;

	buffer* countingBuffer = &d;

//	create the producer thread
	if (st_thread_create(produceChar, producer, 0, 0) == NULL) {
		perror("st_thread_create for producer thread failure");
		exit(1);
	}
	
//	create the star consumer thread
	if (st_thread_create(consumeStar, starConsumer, 0, 0) == NULL) {
		perror("st_thread_create for consumer thread failure");
		exit(1);
	}

//	create the new line consumer thread
	if (st_thread_create(consumeNewLine, newLineBuffer, 0, 0) == NULL) {
		perror("st_thread_create for consumer thread failure");
		exit(1);
	}

//	create the new line consumer thread
	if (st_thread_create(countChars, countingBuffer, 0, 0) == NULL) {
		perror("st_thread_create for consumer thread failure");
		exit(1);
	}


	st_thread_exit(NULL);
	return 0;

}

void* produceChar(void *producer){
	char curChar;
	while ((curChar = getchar()) != EOF){
		depositBuf(producer, curChar);
	}
	depositBuf(producer, curChar);
	st_thread_exit(NULL);
}

void* consumeStar(void *starConsume){
	char temp;
	buffer* starConsumer = starConsume;
	buffer *newLineBuffer = starConsumer->nextBuffer;
	while((temp = removeBuf(starConsumer)) != EOF){
		//if the first char is not a star, just put it in the next buffer
		if (temp == '*'){
			char temp2;
			//but if it is a star, check the next char
			if ((temp2 = removeBuf(starConsumer)) != EOF){
				//if also a char, put in a carrot
				if (temp2 == '*'){
					depositBuf(newLineBuffer, '^');//replace with next buffer
				}	
				//else just add both chars to the next buffer. 
				else{
					depositBuf(newLineBuffer, temp);//replace with next buffer
					depositBuf(newLineBuffer, temp2);		
				}
			}
		}
		else{
			depositBuf(newLineBuffer, temp);
		}
	}
	//need to kill by passing along EOF
	depositBuf(newLineBuffer, temp);
	st_thread_exit(NULL);
}

void* consumeNewLine(void *newLineBuf){
	char temp;
	buffer* newLineBuffer = newLineBuf;
	buffer *countingBuffer = newLineBuffer->nextBuffer;
	while((temp = removeBuf(newLineBuffer)) != EOF){
		//if the first char is not a star, just put it in the next buffer
		if (temp == '\n'){
			depositBuf(countingBuffer, ' ');
		}
		else{
			depositBuf(countingBuffer, temp);
		}
	}
	depositBuf(countingBuffer, temp);
	st_thread_exit(NULL);
}

void* countChars(void *countingBuf){
	char temp;
	char charOutput[LINE_LENGTH];
	int count = 0;
	int lineLength = LINE_LENGTH;
	buffer* countingBuffer = countingBuf;
	while((temp = removeBuf(countingBuffer)) != EOF){
		charOutput[count] = temp;
		if (count == LINE_LENGTH - 1){
			printf("%.80s\n", charOutput);
			count = 0;
		}
		else{
			count++;
		}
	}
	st_thread_exit(NULL);
}


