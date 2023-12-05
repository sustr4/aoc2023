#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
//#define INPUT "input.txt"
#define MAXX 8
#define MAXY 140
#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int from;
	int to;
	int range;
} TMap;

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
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
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
TMap **readInput() {
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
	// TMap *inst=(TMap*)calloc(MAXX, sizeof(TMap));

	// Allocate a two-dimensional arrray of chars
	int x=0,y;
	TMap **inst=calloc(MAXY,sizeof(TMap*));
	for(int iter=0; iter<MAXY; iter++) inst[iter]=calloc(MAXX,sizeof(TMap));


	x=0; y=0;
	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read tokens from single line
		if(!strncmp(line,"seeds:",6)) {
			char *token;
			token = strtok(line+7, " ");
			inst[x][y].to=atoi(token);
			inst[x][y++].range=1;
			while( 1 ) {
				if(!(token = strtok(NULL, " "))) break;
				inst[x][y].to=atoi(token);
				inst[x][y++].range=1;
			}
		}
		else if((line[0]>='a') && (line[0]<='z')) {
			x++; // letter, next column
			y=0;
			printf("Starting col %d: %s\n", x, line);
		}
		else if((line[0]>='0') && (line[0]<='9')) { // Number, read into array
			assert(x>=0);
			sscanf(line,"%d %d %d",
				&(inst[x][y].to),
				&(inst[x][y].from),
				&(inst[x][y].range));
			count++;
			y++;
			printf(".");
		}


	}

	printf("Read %d items into %d columns\n", count, x);

	fclose(input);
	if (line)
	free(line);

//	printMap(map);

	return inst;
}

int findNext(int from, int col, TMap **array) {

	int i;

}

int main(int argc, char *argv[]) {

	TMap **array;
	int i=0,x;	
	array = readInput();

	for(x=0; x<MAXX; x++) {
		
		printf("Column %d\n", x);
		for(i=0; array[x][i].range; i++) {
			printf("%d -> %d (%d)\n", array[x][i].from, array[x][i].to, array[x][i].range);
		}
		printf("\n");
	}
	return 0;
}
