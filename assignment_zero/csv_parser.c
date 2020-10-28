#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
const int max_length = 256;
//
// col 1 -> col 2 -> col3 -> ... -> col n
// item 1
//	|
// item 2
//  |
// item 3
//  |
// ...
// item n
typedef struct Node{
	struct Node * next;
	struct LList * values;
 
	char content[max_length];
} Node;
 
typedef struct LList
{
	Node * head;
	Node * tail;
 
	int size;
 
	void (*transform)(Node *);
} LList;
 
void printList(LList * list);
Node * search(LList * list, char * string);
int next_field(FILE * csv, char * buffer, int max_len);
 
int main(int argc, const char * argv[]){
 
	if (argc < 2){
		printf("usage: %s <csvfile>\n", argv[0]);
		exit(-1);
	}
 
	FILE * csv;
	if( !(csv = fopen(argv[1], "r") )){
		printf("Can't open file %s\n", argv[1]);
		exit(-1);
	}
 
	char buffer[max_length];
	int retval = next_field(csv, buffer, max_length);
 
	LList * headers = malloc(sizeof(LList));
	Node * next = malloc(sizeof(Node));
	headers->head = next;
	headers->size = 1;
	strcpy(next->content, buffer);
	
	while(retval != 1){
		retval = next_field(csv, buffer, max_length);
		next->next = malloc(sizeof(Node));
		next = next->next;
		strcpy(next->content, buffer);
		headers->size++;
	}
 
	headers->tail = next;
	next = headers->head;
	retval = next_field(csv, buffer, max_length);
 
	while(retval != -1){
		if(next == NULL) next = headers->head;
 
		if(next->values == NULL){
			next->values = malloc(sizeof(LList));
			next->values->head = malloc(sizeof(Node));
			next->values->tail = next->values->head;
			next->values->size = 1;
		}
 
		next->values->tail->next = malloc(sizeof(Node));
		strcpy(next->values->tail->content, buffer);
		retval = next_field(csv, buffer, max_length);
		next->values->tail = next->values->tail->next;
		next->values->size++;
		next = next->next;
	}
 
 
	printList(headers);
 
}
 
void printList(LList * list){
	Node * next = list->head;
	while(next != NULL){
		printf("%s\n", next->content);
		printf("----------------------\n");
		Node * nnext = next->values->head;
		while(nnext != NULL){
			printf("%s\n", nnext->content);
			nnext = nnext->next;
		}
		next = next->next;
	}
}
 
Node * search(LList * list, char * string){
	Node * next = list->head;
 
	while(next != NULL && strcmp(next->content, string) != 0){
		next = next->next;
	}
 
	return next;
}
 
int next_field(FILE * csv, char * buffer, int max_len){
 
	char c = fgetc(csv);
	int i = 0;
	bool inQuote = false;
 
	while(i<max_len){
 
		if(c == ',' && !inQuote){
			break;
		}
		if(c == '"'){
			inQuote = !inQuote;
		}
		if(c == '\n' || c == EOF ){
			break;
		}
 
		buffer[i++] = c;
		c = fgetc(csv);
	}
 
	buffer[i] = '\0';
	if(c == EOF) return -1;
	if( c == '\n') return 1;
 
	return 0;
}
