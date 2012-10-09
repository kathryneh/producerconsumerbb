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

#define DEFAULT_OUT stdout

void* produceChar(void *s);
void* consumeStar(void *s);
void* consumeNewLine(void *s);

int main(int argc, char const *argv[]) {

	st_init();
	FILE *output = DEFAULT_OUT;
	buffer b;
	createBuffer(&b);

	buffer* starConsumer = &b;
	buffer* producer = &b;

	buffer c;
	createBuffer(&c);
	buffer* spaceProducer = &c;
	buffer* spaceConsumer = &c;

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

void* consumeStar(void *starConsumer){
	char temp;
	while((temp = removeBuf(starConsumer)) != EOF){
		//if the first char is not a star, just put it in the next buffer
		if (temp == '*'){
			char temp2;
			//but if it is a star, check the next char
			if (temp2 = removeBuf(starConsumer) != EOF){
				//if also a char, put in a carrot
				if (temp2 == '*'){
					putchar('^');//replace with next buffer
				}	
				//else just add both chars to the next buffer. 
				else{
					putchar(temp);//replace with next buffer
					putchar(temp2);		
				}
			}
		}
		else{
			putchar(temp);
		}
	}
	st_thread_exit(NULL);
}
