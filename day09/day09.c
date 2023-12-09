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
//#define MAXX 10
//#define MAXY 10

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


// Print a two-dimensional array
void printMap (int **map) {
	int x,y;
	for(y=0; map[y][0]; y++) {
		for(x=0; x<map[y][0]; x++) {
			printf("%2d%s", map[y][x], x?" ":": ");
		}
		printf("\n");
	}
	printf("\n");
}
// Full block character for maps █

// Retrieve nth neighbor from a map
int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1};
int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1};
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<8));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
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

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
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

//	TPoint *array;
	int i=0,l,sum=0;	
//	array = readInput();
	int **map=readInput();

	printMap(map);
//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(l=0; map[l][0]; l++) {

//		printf("Alloc line %d, size %dx%d\n", l, map[l][0]+1, map[l][0]+1);
		int **hist=calloc(map[l][0]+1,sizeof(int*));
		for(int iter=0; iter<=map[l][0]; iter++) hist[iter]=calloc(map[l][0]+1,sizeof(int));

//		printf("First value of first history appears to be %d\n", hist[1][0]);

		// Copy the first line
		for(i=0; i<map[l][0]; i++) hist[0][i]=map[l][i];
		
		// Count te diffs
		int h = 0;
		while( 1 ) {
			int allNull=1;
			for(i=1; i<hist[h][0]-1; i++) {
//				printf("Diffing %d (%d) vs. %d (%d).\n", i, hist[h][i], i+1, hist[h][i+1]);
				hist[h+1][i]=hist[h][i+1]-hist[h][i];
				if(hist[h+1][i]) allNull = 0;
			}
			hist[++h][0]=i;
			if(allNull) break;
//		printMap(hist);
		}

		printMap(hist);

		printf("Now from history no. %d back\n\n", h);

		hist[h][0]++;
		int newi=0;
		for(h=h-1; h>=0; h--) {
			newi = hist[h][0]++;
			hist[h][newi] = hist[h][newi-1] + hist[h+1][newi-1];

//			printf("Summing %d with %d\n", hist[h][newi-1], hist[h+1][newi-1]);
		}

		printMap(hist);

		sum+=hist[0][newi];

		for(int iter=0; iter<=map[l][0]; iter++) free(hist[iter]);
		free(hist);

	}

	printf("Sum: %d\n", sum);

	return 0;
}
