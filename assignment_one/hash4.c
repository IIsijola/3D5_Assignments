#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const int ARRAY_SIZE = 99991;
const int MAX_STRING_SIZE = 60;

int name_collisions = 0;
int total_unique_names = 0;

struct datum{
	char value[MAX_STRING_SIZE];
	int count;
	struct LinkedList * list;
};

struct LinkedList{
	struct Node * head;
	struct Node * tail;
};

struct Node{
	int age;
	int personId;
	char surname[MAX_STRING_SIZE];
	char forename[MAX_STRING_SIZE];
	char depositionId[MAX_STRING_SIZE];
	char personType[MAX_STRING_SIZE];

	struct Node * next;
};

struct datum * hashTable[ARRAY_SIZE];

void stats();

// int hash(char * surname);
void load(FILE * csv);
void delete(char * surname);
int read_name(FILE * csv, char * buffer, int max_len);
unsigned long hash(char * surname);
unsigned long aux_hash(char * surname);
struct datum * search(char *surname);
void insert(char * surname, char * forename, int id, int age, char * deposition, char * type);
void printList(struct LinkedList * list);

int main(int argc, char const *argv[])
{

	FILE * csv;
	if( !(csv = fopen("people.csv", "r") )){
		printf("Can't open file %s\n", argv[1]);
		exit(-1);
	}

	load(csv);

	stats();
	printf("Names loaded\n");
	printf("Enter quit to quit\n");
	char input[MAX_STRING_SIZE];

	while(1){
		printf(">>> ");
		scanf("%s", input);

		if(strcmp(input, "quit")==0){
			return 1;
		}else if(strcmp(input, "stats")==0){
			stats();
		}else{
			struct datum * item = search(input);
			if(item != NULL){
				printf("%s %d\n", item->value, item->count);
				printList(item->list);
			}else{
				printf("Unknown %s\n", input);
			}
		}
	}
	
	return 0;
}

void load(FILE * csv){
// Person ID,Deposition ID,Surname,Forename,Age,Person Type,Gender,Nationality,Religion,Occupation

	int endline;
	char buffer[MAX_STRING_SIZE];

	printf("Working\n");
	while(endline != 1){
		endline = read_name(csv, buffer, MAX_STRING_SIZE);
	}

	printf("Still working\n");
	while(endline!=-1){

		// declare variables in loops so we don't have to worry about them
		// interfering with anything later on

		int id;
		int age;
		char type[MAX_STRING_SIZE];
		char surname[MAX_STRING_SIZE];
		char forename[MAX_STRING_SIZE];
		char deposition[MAX_STRING_SIZE];

		endline = read_name(csv, buffer, MAX_STRING_SIZE);
		id = atoi(buffer);
		endline = read_name(csv, deposition, MAX_STRING_SIZE);
		endline = read_name(csv, surname, MAX_STRING_SIZE);
		endline = read_name(csv, forename, MAX_STRING_SIZE);
		endline = read_name(csv, buffer, MAX_STRING_SIZE);
		age = atoi(buffer);
		endline = read_name(csv, type, MAX_STRING_SIZE);

		insert(surname, forename, id, age, deposition, type);
		// printf("%s %s %d %d %s %s\n", surname, forename, id, age, deposition, type);
		// the rest of the values we do not care about
		// printf("Still working again\n");
		while(endline != 1 && endline !=-1){
			endline = read_name(csv, buffer, MAX_STRING_SIZE);
		}

	}
	
}
struct datum * search(char *surname){
	int key = hash(surname) % ARRAY_SIZE;
	int i = 0;

	while( hashTable[key] != NULL ){
		if( strcmp(hashTable[key]->value, surname) == 0 ){
			return hashTable[key];
		}
		key = hash(surname) + ( (++i) * aux_hash(surname));
		key %= ARRAY_SIZE;

	}

	return NULL;
}

void insert(char * surname, char * forename, int id, int age, char * deposition, char * type){
	unsigned long key = hash(surname) % ARRAY_SIZE;
	int i = 0;

	// printf("Inside insert!!\n");
	total_unique_names++;
	while( hashTable[key] != NULL ){
		if( strcmp(hashTable[key]->value, surname) == 0 ){
			hashTable[key]->count++;
			// append new node to end of linkedlist
			struct Node * node = malloc(sizeof(struct Node));

			node->personId = id;
			node->age = age;
			strcpy(node->surname, surname);
			strcpy(node->forename, forename);
			strcpy(node->depositionId, deposition);
			strcpy(node->personType, type);

			hashTable[key]->list->tail->next = node;
			hashTable[key]->list->tail = node;
			// printf("Appended\n");
			return;
		}else{
			unsigned long old = key;
			key = hash(surname) + ( (++i) * aux_hash(surname));
			key %= ARRAY_SIZE;
			name_collisions++;
			if (old ==key)
			{
				printf("Key value didn't change for %s\n", surname);
			}
		}

	}

	struct datum * insertion = malloc(sizeof(struct datum));
	struct LinkedList * mList = malloc(sizeof(struct LinkedList));
	struct Node * head = malloc(sizeof(struct Node));

	head->personId = id;
	head->age = age;
	strcpy(head->surname, surname);
	strcpy(head->forename, forename);
	strcpy(head->depositionId, deposition);
	strcpy(head->personType, type);

	insertion->list = mList;
	insertion->list->head = head;
	insertion->list->tail = head;

	strcpy(insertion->value, surname);
	insertion->count = 1;

	hashTable[key] = insertion;
	// printf("added\n");
}

//SMBHash

unsigned long hash(char* surname) {
	unsigned int hash = 0;
	unsigned int i = 0;
	int length = strlen(surname);

	for (i = 0; i < length; surname++, i++)
	{
		hash = (*surname) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

unsigned long aux_hash(char * surname){
	unsigned long hash;
	while(*surname){
		hash = (hash + *surname);
		surname++;
	}
	return hash;


}


int read_name(FILE * csv, char * buffer, int max_len){
 
	char c = fgetc(csv);
	int i = 0;
	bool inQuote = false;
 
	while(i<max_len){
 
		if(c == ',' && !inQuote){
			break;
		}
		if(c == '\n' || c == EOF ){
			break;
		}
 
 		if(c == '"'){
			inQuote = !inQuote;
		}else{
			buffer[i++] = c;
		}
		c = fgetc(csv);
	}
 
	buffer[i] = '\0';
	if(c == EOF) return -1;
	if( c == '\n') return 1;
 
	return 0;
}

void stats(){
	printf("Total capacity %d\n", ARRAY_SIZE);
	printf("Total unique names %d\n", total_unique_names);
	printf("Total name collisions %d\n", name_collisions);
	printf("Load %.2f\n", ((float)total_unique_names/ARRAY_SIZE));

	int frequency = 0;
	struct datum * most_popular_name;

	for (int key = 0; key < ARRAY_SIZE; ++key)
	{
		if(hashTable[key] == NULL) continue;
		if(hashTable[key]->count > frequency){
			most_popular_name = hashTable[key];
			frequency = hashTable[key]->count;
		}

	}

	printf("Most popular name is %s\n", most_popular_name->value);
}

 
void printList(struct LinkedList * list){
	struct Node * next = list->head;
	printf("%20s\t%20s\t%20s\t%20s\t%20s\t%20s\n", "Person ID", "Deposition ID","Surname", "Forename", "Person Age", "Person Type");
	while(next != NULL){
		printf("%20d\t%20s\t%20s\t%20s\t%20d\t%20s\n", next->personId, next->depositionId, next->surname, next->forename, next->age, next->personType);
		next = next->next;
	}
}