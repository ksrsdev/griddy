#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Goal is a program that accepts any number of user inputs then prints them back in same order
//Challenge is to use linked lists NOT arrays, store the data in the heap and not leak any memory.

typedef struct Node {
	char *string;
	struct Node *next;
} Node;

int main(void)
{
	Node *head = NULL;
	Node *tail = NULL;
	char buffer[64];
	bool stillLooping = true;
	const char *QUIT = "QUIT";
	const char *quit = "quit";
	head = tail;
	while (stillLooping) {
		fgets(buffer, 64, stdin);
		buffer[strcspn(buffer, "\n")] = 0;
		if (!strcmp(buffer, quit) || !strcmp(buffer, QUIT)) {
			stillLooping = false;
			break;
		}
		Node *temp = (Node *)malloc(sizeof(Node));
		temp->string = strdup(buffer);
		temp->next = NULL;
		if (head == NULL) {
			head = temp;
			tail = temp;
		} else {
			tail->next = temp;
			tail = temp;
		}
	}
	while (head != NULL) {
		printf("%s\n", head->string);
		Node *next = head;
		head = head->next;
		free (next->string);
		free (next);
	}
	tail = NULL;
	return 0;
}

//A- 
//Notes for improvement: 
//						remove stillLooping bool, just use while(true)
//						handle NULL input from fgets and strdup (malloc)

//NEXT: Move the logic out of main()
