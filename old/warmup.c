//COMP 530 Assignment 1
//Author: Katie Hawthorne
//Collaborators: Michael Barlock (shared tests)

#include <stdio.h>

int main(int argc, const char * argv[]) {

    int numChar = 0;
    char curChar;
    char charOutput[80];
	
    //begin the loop for input
    while ((curChar = getchar()) != EOF){
		
        //first check to see if there is an asterisk
        if (curChar == '*'){
			//now if there was previously an asterisk..
			if (numChar > 0 && charOutput[numChar-1] == '*') {
				numChar--;
				curChar = '^';
			}
        }
		
		if (numChar >= 80){
			//limit printf to 80 characters
            printf("%.80s\n", charOutput);
            //start over at the beginning
            numChar = 0;
		}
		
		//replace newlines with spaces
        if (curChar == '\n') charOutput[numChar] = ' ';
		//save the character to the array
		else charOutput[numChar] = curChar;
        numChar++;
		// if numChar is never reached, it's never printed out
    }
}

