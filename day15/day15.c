#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 4000
//#define INPUT "unit1.txt"

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

// Comparator function example
int comp(const void *a, const void *b)
{
	const int *da = (const int *) a;
	const int *db = (const int *) b;
	return (*da > *db) - (*da < *db);
}

// Example for calling qsort()
//qsort(array,count,sizeof(),comp);




// Read input file line by line (e.g., into an array)
char **readInput() {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	char **map=calloc(MAXX,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

		// Read tokens from single line
		char *token;
		token = strtok(line, ",");
		map[count++]=strdup(token);
		while( 1 ) {
			if(!(token = strtok(NULL, ","))) break;
			map[count++]=strdup(token);
		}
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
	int i=0, sum=0, sumsum=0;	
//	array = readInput();
	char **map=readInput();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; map[i]; i++) {

//    Determine the ASCII code for the current character of the string.
//    Increase the current value by the ASCII code you just determined.
//    Set the current value to itself multiplied by 17.
//    Set the current value to the remainder of dividing itself by 256.

		sum=0;
		for(int y=0; y<strlen(map[i]); y++) {
			sum+=(int)(map[i][y]);
//printf("\t%d\n", sum);
			sum*=17;
//printf("\t%d\n", sum);
			sum=sum % 256;
//printf("\t%d\n", sum);
		}

		sumsum+=sum;

		printf("%s\tbecomes %d\t%d\n", map[i], sum, sumsum);

	}

	return 0;
}
