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
void printScore (int **map, int **score, TPoint *old) {
	int x,y;
	int **went=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) went[iter]=calloc(MAXX,sizeof(int));
	went[MAXY-1][MAXY-1]=1;
	for(int trythis=0; trythis<500; trythis++) {
		went[old[trythis].y][old[trythis].x]=1;
		if((old[trythis].x==MAXX-1) && (old[trythis].y==MAXY-1)) break;
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

int newx(int x, int steps, int d) { // New X based on direction
	switch(d) {
		case 0: return x+steps;
		case 2: return x-steps;
		case 1:
		case 3: return x;
	}
	return x;
}
int newy(int y, int steps, int d) { // New X based on direction
	switch(d) {
		case 1: return y+steps;
		case 3: return y-steps;
		case 0:
		case 2: return y;
	}
	return y;
}

int step(int x, int y, int d, int **map, int **score, TPoint *old, int depth, int s) {

	static int iteration=0;
	int nx, ny, nd;

	old[depth].x=x;
	old[depth].y=y;
	iteration++;
//	if(iteration>8000) return 0;

//	printf("\nIteration %d: [%d,%d], direction %d, depth %d, score %d\n", iteration, x, y, d, depth, s);
	if((y<0) || (x<0) || (y>=MAXY) || (x>=MAXX)) {
//		printf("Out of bounds\n");
		return 0;
	}

	if((score[y][x]) && (s>=score[y][x])) {
//		printf("Score exceeded\n");
		return 0; // Already been here with a better score
	}


	score[y][x]=s; // This is the best score on the way here so far

//	printScore(map,score,old);

	if((depth==2) && (y==1) && (x==2)) {

		printf("\nIteration %d: [%d,%d], direction %d, depth %d, score %d\n", iteration, x, y, d, depth, s);
		printScore(map,score,old);
	}

	if((y==MAXY-1) && (x==MAXX-1)) {
//		printf("No need to search beyond the goal\n");
//		printf("\n");
//		printScore(map,score,old);
		return 0;
	}

	// Try left
	nd=d-1; if(nd<0) nd=3;
	int sadd=0;
	for(int stepNo=1; stepNo<=3; stepNo++) {
//		printf("Trying %d left\n", stepNo);
		nx=newx(x, stepNo, nd); ny=newy(y, stepNo, nd);
		if((ny<0) || (nx<0) || (ny>=MAXY) || (nx>=MAXX)) break;
//		if((y==0) && (x==0) && (nx==3)) {
//			printf("\nThis is the interesting point in L (depth %d)\n", depth);
//			printScore(map,score,old);
//		}
		sadd+=map[ny][nx];
//		printf("Adding %d to score (=%d)\n", sadd, s+sadd);
		step(nx, ny, nd, map, score, old, depth+1, s+sadd);
	}

	// Try right
	nd=d+1; if(nd>3) nd=0;
	sadd=0;
	for(int stepNo=1; stepNo<=3; stepNo++) {
//		printf("Trying %d right\n", stepNo);
		nx=newx(x, stepNo, nd); ny=newy(y, stepNo, nd);
//		if((y==0) && (x==0) && (nx==3)) {
//			printf("\nThis is the interesting point in R (depth %d)\n", depth);
//			printScore(map,score,old);
//		}
		if((ny<0) || (nx<0) || (ny>=MAXY) || (nx>=MAXX)) break;
		sadd+=map[ny][nx];
//		printf("Adding %d to score\n", sadd);
		step(nx, ny, nd, map, score, old, depth+1, s+sadd);
	}

	return 0;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
//	int i=0;	
//	array = readInput();
	int **map=readInput();
	int **score=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) score[iter]=calloc(MAXX,sizeof(int));
	TPoint *old=calloc(1000,sizeof(TPoint*));

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	step(0,0,1,map,score,old,0,map[0][0]);
	step(0,0,0,map,score,old,0,map[0][0]);


	printf("\n");
	printScore(map,score,old);

	return 0;
}
