
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Goal is a program that accepts any number of user inputs then prints them back in reverse order
//Challenge is to use linked lists NOT arrays, store the data in the heap and not leak any memory.
//

typedef struct Node {
	char *string;
	struct Node *next;
} Node;

int main(void)
{
	Node *head = NULL;
	//Allocate memory for head Node
	char buffer[64]; //buffer for string to be copied to Node data
	bool stillLooping = true; //read input from user
	//These strings, when entered, cause the program to stop seeking input and move on to the printing section
	const char *QUIT = "QUIT";
	const char *quit = "quit";
	//Input loop
	while (stillLooping) {
		//read input from user into buffer
		fgets(buffer, 64, stdin);
		//sanitize string in buffer
		buffer[strcspn(buffer, "\n")] = 0;
		//check if input was a terminator
		if (!strcmp(buffer, quit) || !strcmp(buffer, QUIT)) {
			//if so terminate input loop and move to printing
			stillLooping = false;
			break;
		}
		//Else we're going to save the input
		//create new Node
		Node *temp = (Node *)malloc(sizeof(Node));
		//copy string into new Node data space
		//NOTE: OG head will be NULL
		temp->string = strdup(buffer);
		//new Node next is head 
		temp->next = head;
		//head points to new node
		head = temp;
		//clear buffer
	}
	//Here is the print and free section 
	//After Input print the elements in backwards order
	while (head != NULL) {
		//print current node string
		printf("%s\n", head->string);
		//a pointer to the next node
		Node *next = head->next;
		//free current node
		free (head->string);
		free (head);
		head = next;
	}
	return 0;
}
