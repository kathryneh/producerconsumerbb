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
void* consumeChar(void *s);

int main(int argc, char const *argv[]) {

	st_init();
	FILE *output = DEFAULT_OUT;
	buffer b;
	createBuffer(&b);

	buffer* consumer = &b;
	buffer* producer = &b;

//	create the producer thread
	if (st_thread_create(produceChar, producer, 0, 0) == NULL) {
		perror("st_thread_create for producer thread failure");
		exit(1);
	}
	
//	create the consumer thread
	if (st_thread_create(consumeChar, consumer, 0, 0) == NULL) {
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

void* consumeChar(void *s){
	char temp;
	buffer* consumer = s;
	while((temp = removeBuf(consumer)) != EOF){
		putchar(temp);
	}
	st_thread_exit(NULL);
}
