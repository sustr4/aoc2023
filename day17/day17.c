#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 141
#define MAXY 141
//#define INPUT "unit1.txt"
//#define MAXX 13
//#define MAXY 13

int startMax=INT_MAX;

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
void printScore (int **map, int ****score) {
	int x,y;
	int **went=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) went[iter]=calloc(MAXX,sizeof(int));


	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
                       int min=INT_MAX;
                       for(int d=0; d<4; d++) {
                               for(int c=1; c<=3; c++) {
                                       if((score[y][x][d][c]) && (score[y][x][d][c]<min)) min=score[y][x][d][c];
                               }
                       }
			if(went[y][x]) printf("\033[1;34m");
                       if(min==INT_MAX) printf("___");
                       else printf("%3d", min);
			printf("\033[0;31m%d\033[0m ", map[y][x]);
		}
		printf("\n");
	}
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

int step(int x, int y, int d, int steps, int **map, int ****score, int depth, int s) {

//	static int iteration=0;
	int nx, ny, nd;

//	if(startMax<136) return 0;
//	iteration++;
//	if(iteration>8000) return 0;

//	printf("\nIteration %d: [%d,%d], direction %d, depth %d, score %d\n", iteration, x, y, d, depth, s);
	if((y<0) || (x<0) || (y>=MAXY) || (x>=MAXX)) return 0;

	if(s>startMax) return 0;

	if((score[y][x][d][steps]) && (s>=score[y][x][d][steps])) return 0; // Already been here with a better score

	score[y][x][d][steps]=s; // This is the best score on the way here so far

	if((y==MAXY-1) && (x==MAXX-1)) {
		startMax=s;
		printf("Found better minimum: %d\n", startMax);
		printScore(map,score);
		return 0;
	}

	// Try left
	nd=d-1; if(nd<0) nd=3;
	int sadd=0;
	for(int stepNo=1; stepNo<=3; stepNo++) {
//		printf("Trying %d left\n", stepNo);
		nx=newx(x, stepNo, nd); ny=newy(y, stepNo, nd);
		if((ny<0) || (nx<0) || (ny>=MAXY) || (nx>=MAXX)) break;
		sadd+=map[ny][nx];
//		printf("Adding %d to score (=%d)\n", sadd, s+sadd);
		step(nx, ny, nd, stepNo, map, score, depth+1, s+sadd);
	}

	// Try right
	nd=d+1; if(nd>3) nd=0;
	sadd=0;
	for(int stepNo=1; stepNo<=3; stepNo++) {
//		printf("Trying %d right\n", stepNo);
		nx=newx(x, stepNo, nd); ny=newy(y, stepNo, nd);
		if((ny<0) || (nx<0) || (ny>=MAXY) || (nx>=MAXX)) break;
		sadd+=map[ny][nx];
//		printf("Adding %d to score\n", sadd);
		step(nx, ny, nd, stepNo, map, score, depth+1, s+sadd);
	}

	return 0;
}



int main(int argc, char *argv[]) {

//	TPoint *array;
//	int i=0;	
//	array = readInput();
	int **map=readInput();

        int ****score=calloc(MAXY,sizeof(int***));
        for(int y=0; y<MAXY; y++) {
                score[y]=calloc(MAXX,sizeof(int**));
                for(int x=0; x<MAXX; x++) {
                        score[y][x]=calloc(4,sizeof(int*));
                        for(int d=0; d<4; d++) {
                                score[y][x][d]=calloc(4,sizeof(int));
                        }
                }
        }

	startMax=map[0][0];

	int l=0;
	int y=0, x=0;
	while(1) {
		if(l) { x++; l=!l; }
		else { y++; l=!l; }
		startMax+=map[y][x];
		if((x==MAXX-1) && (y=MAXY-1)) break;
	}

	printf("naiive max: %d\n", startMax);

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	step(0,0,1,3,map,score,0,0);
	step(0,0,2,3,map,score,0,0);


//	printf("\n");
//	printScore(map,score);

	return 0;
}
