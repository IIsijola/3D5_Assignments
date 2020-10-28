#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const int ARRAY_SIZE = 1024;
const int MAX_STRING_SIZE = 30;

int name_collisions = 0;
int total_unique_names = 0;

struct datum{
	int key;
	char value[MAX_STRING_SIZE];
	int count;
};

struct datum * hashTable[ARRAY_SIZE];

void stats();
int hash(char * surname);
void insert(char * surname);
void delete(char * surname);
int read_name(FILE * csv, char * buffer, int max_len);
struct datum * search(char *surname);

int main(int argc, char const *argv[])
{

	FILE * csv;
	if( !(csv = fopen("names.csv", "r") )){
		printf("Can't open file %s\n", argv[1]);
		exit(-1);
	}

	char buffer[MAX_STRING_SIZE];
	int endline = 0;

	while(!endline){
		endline = read_name(csv, buffer, MAX_STRING_SIZE);
		insert(buffer);
	}

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
			}else{
				printf("Unknown %s\n", input);
			}
		}
	}
	

	return 0;
}

struct datum * search(char *surname){
	int key = hash(surname);

	while( hashTable[key] != NULL ){
		if( strcmp(hashTable[key]->value, surname) == 0 ){
			return hashTable[key];
		}
		key++;
		key %= ARRAY_SIZE;

	}

	return NULL;
}

void insert(char * surname){
	int key = hash(surname);

	while( hashTable[key] != NULL ){
		if( strcmp(hashTable[key]->value, surname) == 0 ){
			hashTable[key]->count++;
			return;
		}else{
			name_collisions++;
		}
		key++;
		key %= ARRAY_SIZE;

	}

	total_unique_names++;
	struct datum * insertion = malloc(sizeof(struct datum));
	strcpy(insertion->value, surname);
	insertion->key = key;
	insertion->count = 1;
	hashTable[key] = insertion;	
}

void delete(char * surname){
	int key = hash(surname);

	while( hashTable[key] != NULL ){
		if( strcmp(hashTable[key]->value, surname) == 0 ){
			free(hashTable[key]);
			hashTable[key] = NULL;
		}
		key++;
		key %= ARRAY_SIZE;
	}

}

int hash(char * surname){
	int hash = 0;
	while(*surname){
		hash = (hash + *surname);
		surname++;
	}
	return (hash % ARRAY_SIZE);
}

int read_name(FILE * csv, char * buffer, int max_len){

	char c = fgetc(csv);
	int i = 0;
	bool inQuote = false;

	while(i<max_len){

		if(c == ',' && !inQuote){
			break;
		}
		if(c == '"'){
			inQuote = !inQuote;
			// printf("quote found\n");
		}
		if(c == '\n' || c == EOF ){
			break;
		}

		buffer[i++] = c;
		c = fgetc(csv);
	}

	buffer[i] = '\0';
	if(c == EOF) return 1;

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