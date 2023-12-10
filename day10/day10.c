#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 140
#define MAXY 140
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int south;
	int north;
	int east;
	int west;
	int dist;
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
void printDijk (TPoint **dijk) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(dijk[y][x].dist) printf("%d", dijk[y][x].dist%10);
			else printf(" ");
		}
		printf("\n");
	}
}
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

//	printMap(map);

	return map;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
//	int i=0;	
//	array = readInput();
	char **map=readInput();

	int x=0, y=0;

	// Set up distance matrix	
	TPoint **dijk=calloc(MAXY,sizeof(TPoint*));
	for(int iter=0; iter<MAXY; iter++) dijk[iter]=calloc(MAXX,sizeof(TPoint));
	// Fill connections	
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			switch(map[y][x]) {
			case '|':
				dijk[y][x].north = 1;
				dijk[y][x].south = 1;
				break;
			case '-':
				dijk[y][x].east = 1;
				dijk[y][x].west = 1;
				break;
			case 'L':
				dijk[y][x].north = 1;
				dijk[y][x].east = 1;
				break;
			case 'J':
				dijk[y][x].north = 1;
				dijk[y][x].west = 1;
				break;
			case '7':
				dijk[y][x].south = 1;
				dijk[y][x].west = 1;
				break;
			case 'F':
				dijk[y][x].south = 1;
				dijk[y][x].east = 1;
				break;
			case 'S':
				dijk[y][x].dist=1;
				dijk[y][x].north=1;
				dijk[y][x].south=1;
				dijk[y][x].east=1;
				dijk[y][x].west=1;
				break;
			}
		}
	}

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
		}
	}


	int curr = 1;

//	for(int k=0; k<2; k++) {
	while(1) {
		int change=0;

		for(y=0; y<MAXY; y++) {
			for(x=0; x<MAXX; x++) {
				if(dijk[y][x].dist==curr) {
					if((dijk[y][x].north)&&(y>0)&&(dijk[y-1][x].south)&&(!dijk[y-1][x].dist)) {
						dijk[y-1][x].dist = curr+1;
						change++;
					}
					if((dijk[y][x].south)&&(y<MAXY-1)&&(dijk[y+1][x].north)&&(!dijk[y+1][x].dist)) {
						dijk[y+1][x].dist = curr+1;
						change++;
					}
					if((dijk[y][x].west)&&(x>0)&&(dijk[y][x-1].east)&&(!dijk[y][x-1].dist)) {
						dijk[y][x-1].dist = curr+1;
						change++;
					}
					if((dijk[y][x].east)&&(x<MAXX-1)&&(dijk[y][x+1].west)&&(!dijk[y][x+1].dist)) {
						dijk[y][x+1].dist = curr+1;
						change++;
					}

				}
			}
		}

//	printDijk(dijk);
		printf("Round %d: %d changes\n", curr, change);
		if(!change) break;
		curr++;
	}

	printDijk(dijk);

	printf("Furthest %d steps\n", curr-1);

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	return 0;
}
