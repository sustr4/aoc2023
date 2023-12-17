#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
//#define INPUT "input.txt"
//#define MAXX 141
//#define MAXY 141
#define INPUT "unit1.txt"
#define MAXX 13
#define MAXY 13

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
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {


			printf("%d", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █ and border elements ┃━┗┛┏┓
// Color printf("\033[1;31mR \033[1;32mG \033[1;34mB \033[0moff\n");
void printScore (int **map, int **score) {
	int x,y;
	int **went=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) went[iter]=calloc(MAXX,sizeof(int));
	went[MAXY-1][MAXY-1]=1;
	while(1) {
		int c=0;
		for(y=0; y<MAXY; y++) {
			for(x=0; x<MAXX; x++) {
				if((y-1>=0) && (went[y][x]) && (!went[y-1][x]) && (score[y][x]==score[y-1][x]+map[y][x])) { went[y-1][x]=1; c=1; }
				if((y+1<MAXY) && (went[y][x]) && (!went[y+1][x]) && (score[y][x]==score[y+1][x]+map[y][x])) { went[y+1][x]=1; c=1; }
				if((x-1>=0) && (went[y][x]) && (!went[y][x-1]) && (score[y][x]==score[y][x-1]+map[y][x])) { went[y][x-1]=1; c=1; }
				if((x+1<MAXX) && (went[y][x]) && (!went[y][x-1]) && (score[y][x]==score[y][x+1]+map[y][x])) { went[y][x+1]=1; c=1; }
			}
		}
		if(!c) break;
	}

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(went[y][x]) printf("\033[1;34m");
			printf("%3d", score[y][x]);
			printf("\033[0;31m%d\033[0m ", map[y][x]);
		}
		printf("\n");
	}
}

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
	int x=0, y=0;
	int **map=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(int));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x]-'0';
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

	printMap(map);

	return map;
}

// 0 right, 1 down, 2 left, 3 up

int newx(int x, int d) { // New X based on direction
	switch(d) {
		case 0: return x+1;
		case 2: return x-1;
		case 1:
		case 3: return x;
	}
	return x;
}
int newy(int y, int d) { // New X based on direction
	switch(d) {
		case 1: return y+1;
		case 3: return y-1;
		case 0:
		case 2: return y;
	}
	return y;
}

int step(int x, int y, int d, int cd, int **map, int **score, int s) {

	int nx, ny, nd;

	if(cd>=3) return 0; //already went straight too many times

	if((y<0) || (x<0) || (y>=MAXY) || (x>=MAXX)) return 0;

	if((score[y][x]) && (s+map[y][x]>score[y][x])) return 0; // Already been here with a better score

	score[y][x]=s+map[y][x]; // This is the best score on the way here so far

	// Try straight
	nx=newx(x, d); ny=newy(y, d); nd=d;
	step(nx, ny, d, cd+1, map, score, s+map[y][x]);
	
	// Try left
	nd=d-1; if(nd<0) nd=3;
	nx=newx(x, nd); ny=newy(y, nd);
	step(nx, ny, d, 0, map, score, s+map[y][x]);

	// Try right
	nd=d+1; if(nd>3) nd=0;
	nx=newx(x, nd); ny=newy(y, nd);
	step(nx, ny, d, 0, map, score, s+map[y][x]);

	return 0;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
//	int i=0;	
//	array = readInput();
	int **map=readInput();
	int **score=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) score[iter]=calloc(MAXX,sizeof(int));

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	step(0,0,0,0,map,score,0);

	printScore(map,score);

	

	return 0;
}
