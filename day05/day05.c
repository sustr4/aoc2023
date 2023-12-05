#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 8
#define MAXY 140
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	long from;
	long to;
	long range;
} TMap;

// Read input file line by line (e.g., into an array)
TMap **readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a two-dimensional arrray of chars
	int x=0,y;
	TMap **inst=calloc(MAXX,sizeof(TMap*));
	for(int iter=0; iter<MAXX; iter++) inst[iter]=calloc(MAXY,sizeof(TMap));

	x=0; y=0;
	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read tokens from single line
		if(!strncmp(line,"seeds:",6)) { // This is the first line with seed numbers
			char *token;
			token = strtok(line+7, " ");
			inst[x][y].to=atol(token);
			inst[x][y++].range=1;
			while( 1 ) {
				if(!(token = strtok(NULL, " "))) break;
				inst[x][y].to=atol(token);
				inst[x][y++].range=1;
			}
		}
		else if((line[0]>='a') && (line[0]<='z')) { // Here starts another set of mappings
			x++; // next column
			y=0; // from top ;-)
		}
		else if((line[0]>='0') && (line[0]<='9')) { // Number, read into array
			assert(x>=0);
			sscanf(line,"%ld %ld %ld",
				&(inst[x][y].to),
				&(inst[x][y].from),
				&(inst[x][y].range));
			count++;
			y++;
		}


	}

	printf("Read %d items into %d columns\n", count, x);

	fclose(input);
	if (line)
	free(line);

	return inst;
}

long findNext(long from, int col, TMap **array) { // Get a number and find a rule that matches.
	int i;

	for(i=0; array[col][i].range; i++) { // Find rule that applies
		if((from>=array[col][i].from) &&
		   (from<array[col][i].from+array[col][i].range)) {
			long offset=from-array[col][i].from;
//			printf("Rule for %ld: %ld -> %ld (%ld), offset %ld\n", from, array[col][i].from, array[col][i].to, array[col][i].range, offset);
			return(array[col][i].to+offset);
		}
	}
	return from; // Not in list, matches itself
}

long findLocation(long from, int col, TMap **array) { // Find next mapping unless already at the end
	if(col>=MAXX) return from; // No more searching, this is it
	return findLocation(findNext(from,col,array), col+1, array);
}

long min=LONG_MAX;

int main(int argc, char *argv[]) {

	TMap **array;
	long loc;
	array = readInput();

	for(int y=0; array[0][y].range; y+=2) { // Every two numbers
		printf("Try form %ld (range %ld)\n", array[0][y].to, array[0][y+1].to);
		#pragma omp parallel for private(loc) shared(min)
		for(long i=array[0][y].to; i<=array[0][y].to+array[0][y+1].to; i++) {
			loc = findLocation(i,1,array);
			if(loc<min) {
				min=loc;
				printf("Seed number %ld corresponds to location %ld (min. seen so far %ld).\n", i, loc, min);
			}
		}
	}
	return 0;
}
