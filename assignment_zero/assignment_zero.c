#include <stdio.h>

const int max_len = 255;
int next_field(FILE * csv, char * buffer, int max_len);


int main(){
	FILE * csv;
	csv = fopen("pokemon.csv", "r");

	if(csv == NULL){
		perror("Error opening file");
		return -1;
	}

	char buffer[max_len];
	int ret_val = next_field(csv, buffer, max_len);

	while(ret_val != 1){
		printf("%s\n", buffer);
		ret_val = next_field(csv, buffer, max_len);
	}

	return 0;
}


int next_field(FILE * csv, char * buffer, int max_len){

	char c = fgetc(csv);
	int i = 0;

	while( c!=',' && i<max_len){
		if( c == '\n' || c == EOF ){
			break;
		}

		buffer[i++] = c;
		c = fgetc(csv);
	}
	buffer[i] = '\0';
	if(c == EOF) return 1;

	return 0;
}