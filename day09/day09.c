#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 76
#define MAXY 202
//#define INPUT "unit1.txt"

// Print a two-dimensional array
void printMap (int **map, int *prec) {
	int x,y;
	for(y=0; map[y][0]; y++) {
		printf("%d items: ", map[y][0]);
		if (prec) printf("(%2d) ",prec[y]);
		for(x=1; x<map[y][0]; x++) {
			printf("%2d ", map[y][x]);
		}
		printf("\n");
	}
	printf("\n");
}

// Read input file line by line (e.g., into an array)
int **readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	int **map=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(int));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		int i=1;
		map[count][i]=atoi(token);
		while( ++i ) {
			if(!(token = strtok(NULL, " "))) break;
			map[count][i]=atoi(token);
		}
		map[count][0]=i;
		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int main(int argc, char *argv[]) {

	int i=0,l,sum=0,precsum=0;	
	int **map=readInput();

	printMap(map, NULL);
	for(l=0; map[l][0]; l++) {

//		printf("Alloc line %d, size %dx%d\n", l, map[l][0]+1, map[l][0]+1);
		int **hist=calloc(map[l][0]+1,sizeof(int*));
		for(int iter=0; iter<=map[l][0]; iter++) hist[iter]=calloc(map[l][0]+1,sizeof(int));
		int *prec=calloc(map[l][0]+1,sizeof(int)); // Yes, this is an afterthought, but not mine ;-)

		// Copy the first line
		for(i=0; i<map[l][0]; i++) hist[0][i]=map[l][i];
		
		// Count the diffs
		int h = 0;
		while( 1 ) {
			int allNull=1;
			for(i=1; i<hist[h][0]-1; i++) {
//				printf("Diffing %d (%d) vs. %d (%d).\n", i, hist[h][i], i+1, hist[h][i+1]);
				if((hist[h+1][i]=hist[h][i+1]-hist[h][i])) allNull = 0;
			}
			hist[++h][0]=i;
			if(allNull) break;
		}

		printMap(hist, NULL);

		printf("Now from history no. %d back\n\n", h);

		hist[h][0]++;
		int newi=0;
		for(h=h-1; h>=0; h--) { // Count lines back
			newi = hist[h][0]++;
			hist[h][newi] = hist[h][newi-1] + hist[h+1][newi-1];
			prec[h] = hist[h][1] - prec[h+1];
		}

		printMap(hist, prec);

		sum+=hist[0][newi];
		precsum+=prec[0];

		for(int iter=0; iter<=map[l][0]; iter++) free(hist[iter]);
		free(hist);
		free(prec);

	}

	printf("Sum (task 1): %d\n", sum);
	printf("Prec sum (task 2): %d\n", precsum);

	return 0;
}
