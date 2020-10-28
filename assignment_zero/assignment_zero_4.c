#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <string.h>

const int MAX = 650;
const int max_len = 300;
int next_field(FILE * csv, char * buffer, int max_len);

struct pokemon {
	int number;
	char name[max_len];
	char type_one[max_len];
	char type_two[max_len];
	int total;
	int hp;
	int attack;
	int defense;
	int sp_attack;
	int sp_defense;
	int speed;
	int generation;
	char legendary[6];
	char entry[max_len];
};

int main(){
	FILE * csv;
	csv = fopen("pokemon.csv", "r");
	struct pokemon pokemons[MAX]; 
	int num;
	int ret_val = 0;

	while(ret_val!=1){
		char buffer[max_len];
		ret_val = next_field(csv, buffer, max_len);
		int numVal; 

		if(ret_val == 1) break;
		if(buffer[0] == '#'){
			for (int i = 0; i < 14; ++i)
			{
				next_field(csv, buffer, max_len);
			}
		}

		// printf("%s\n", buffer);
		sscanf(buffer, "%d", &pokemons[num].number);
		// printf("%d\n", pokemons[num].number);

		next_field(csv, buffer, max_len);
		strcpy(pokemons[num].name, buffer);

		next_field(csv, buffer, max_len);
		strcpy(pokemons[num].type_one, buffer);

		next_field(csv, buffer, max_len);
		strcpy(pokemons[num].type_two, buffer);

		next_field(csv, buffer, max_len);
		sscanf(buffer, "%d", &pokemons[num].total);

		next_field(csv, buffer, max_len);
		sscanf(buffer, "%d", &pokemons[num].hp);

		next_field(csv, buffer, max_len);
		sscanf(buffer, "%d", &pokemons[num].attack);

		next_field(csv, buffer, max_len);
		sscanf(buffer, "%d", &pokemons[num].defense);

		next_field(csv, buffer, max_len);
		sscanf(buffer, "%d", &pokemons[num].sp_attack);

		next_field(csv, buffer, max_len);
		sscanf(buffer, "%d", &pokemons[num].sp_defense);

		next_field(csv, buffer, max_len);
		sscanf(buffer, "%d", &pokemons[num].speed);

		next_field(csv, buffer, max_len);
		sscanf(buffer, "%d", &pokemons[num].generation);

		next_field(csv, buffer, max_len);
		strcpy(pokemons[num].legendary, buffer);

		next_field(csv, buffer, max_len);
		strcpy(pokemons[num].entry, buffer);

		
		num++;
	}

	fclose (csv);

	for (int j = 0; j < num; ++j)
		{
			printf("Pokemon number %d\n", pokemons[j].number);
			printf("Name %s\n", pokemons[j].name);
			printf("Type one %s\n", pokemons[j].type_one);
			printf("Type two %s\n", pokemons[j].type_two);
			printf("Total %d\n", pokemons[j].total);
			printf("HP %d\n", pokemons[j].hp);
			printf("Attack %d\n", pokemons[j].attack);
			printf("Defense %d\n", pokemons[j].defense);
			printf("Sp. Attack %d\n", pokemons[j].sp_attack);
			printf("Sp. Defense %d\n", pokemons[j].sp_defense);
			printf("Speed %d\n", pokemons[j].speed);
			printf("Generation %d\n", pokemons[j].generation);
			printf("Legendary %s\n", pokemons[j].legendary);
			printf("Entry %s\n", pokemons[j].entry);

			printf("Average attack power %.2f\n", ((float)(pokemons[j].sp_attack + pokemons[j].attack)/2) );
			printf("\n\n\n");



		}
	return 0;
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
	if(c == EOF) return 1;

	return 0;
}