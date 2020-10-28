#include <stdio.h>
#include <stdbool.h> 

const int max_len = 255;
int next_field(FILE * csv, char * buffer, int max_len);


int main(){
	FILE * csv;
	csv = fopen("test.csv", "r");

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