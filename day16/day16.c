#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 110
#define MAXY 110
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TEnergy;

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
void printMap (char **map, char ***energy) {
	int x,y, e;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			e=0;
			for(int i=0; i<4; i++) if(energy[y][x][i]) e = 1;
			if(e) printf("\033[1;31m");
			printf("%c", map[y][x]);
			if(e) printf("\033[0m");
		}
		printf("\n");
	}
}
// Full block character for maps █ and border elements ┃━┗┛┏┓

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
char **readInput() {
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
	// TEnergy *inst=(TEnergy*)calloc(MAXX, sizeof(TEnergy));

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

		// Read tokens from single line
		//char *token;
		//token = strtok(line, ",");
		//while( 1 ) {
		//	if(!(token = strtok(NULL, ","))) break;
		//}

		count++;
	}

	fclose(input);
	if (line)
	free(line);


	return map;
}

int outOfBounds(int y, int x) {
	if(y<0) return 1;
	if(y>=MAXY) return 1;
	if(x<0) return 1;
	if(x>=MAXX) return 1;
	return 0;
}

#define RIGHT dy=0; dx=1; dd=0;
#define DOWN dy=1; dx=0; dd=1;
#define LEFT dy=0; dx=-1; dd=2;
#define UP dy=-1; dx=0; dd=3;
int trace(int y, int x, int d, char **map, char ***energy) {

	int dx=0, dy=0;
	int dd=d;
	// 0 right, 1 down, 2 left, 3 up

	if(outOfBounds(x+dx, y+dy)) return 0;

	if(energy[y][x][d]) return 0; // We have already been here
	energy[y][x][d]=1;

	switch(map[y][x]) {
	case '.':
		switch (d) {
			case 0: RIGHT break;
			case 1: DOWN break;
			case 2: LEFT break;
			case 3: UP break;
		}
		trace(y+dy, x+dx, dd, map, energy);
		break;
	case '/':
		switch (d) { // 0 right, 1 down, 2 left, 3 up
			case 0: UP break;
			case 1: LEFT break;
			case 2: DOWN break;
			case 3: RIGHT break;
		}
		trace(y+dy, x+dx, dd, map, energy);
		break;
	case '\\':
		switch (d) { // 0 right, 1 down, 2 left, 3 up
			case 0: DOWN break;
			case 1: RIGHT break;
			case 2: UP break;
			case 3: LEFT break;
		}
		trace(y+dy, x+dx, dd, map, energy);
		break;
	case '|':
		switch (d) { // 0 right, 1 down, 2 left, 3 up
			case 0: 
			case 2:
				UP
				trace(y+dy, x+dx, dd, map, energy);
				DOWN
				trace(y+dy, x+dx, dd, map, energy);
				break;
			case 1: 
				DOWN
				trace(y+dy, x+dx, dd, map, energy);
				break;
			case 3:
				UP
				trace(y+dy, x+dx, dd, map, energy);
				break;
		}
		break;
	case '-':
		switch (d) { // 0 right, 1 down, 2 left, 3 up
			case 1: 
			case 3:
				LEFT
				trace(y+dy, x+dx, dd, map, energy);
				RIGHT
				trace(y+dy, x+dx, dd, map, energy);
				break;
			case 0: 
				RIGHT
				trace(y+dy, x+dx, dd, map, energy);
				break;
			case 2:
				LEFT
				trace(y+dy, x+dx, dd, map, energy);
				break;
		}
		break;
	}
	

	return 0;
}

int count(char ***energy) {
	int cnt=0;
	for(int y=0; y<MAXY; y++)
		for(int x=0; x<MAXX; x++)
			for(int d=0; d<4; d++)
				if(energy[y][x][d]) {
					cnt++;
					break;
				}
	return cnt;
}

int main(int argc, char *argv[]) {

//	TEnergy *array;
//	int i=0;	
//	array = readInput();
	char **map = readInput();
	char ***energy = calloc(MAXY,sizeof(char**));
	for(int iter=0; iter<MAXY; iter++) {
		energy[iter]=calloc(MAXX,sizeof(char*));
		for(int yter=0; yter<MAXX; yter++) energy[iter][yter]=calloc(4,sizeof(char));
	}

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	trace(0, 0, 0, map, energy);

	printMap(map, energy);

	printf("Energized fields: %d\n", count(energy));
	return 0;
}
