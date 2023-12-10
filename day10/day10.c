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
//#define MAXY 9

// Point structure definition
typedef struct {
	int south;
	int north;
	int east;
	int west;
	int dist;
} TPoint;

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

// Print an enlarged map
void printThree (char **map) {
	int x,y;
	for(y=0; y<MAXY*3; y++) {
		for(x=0; x<MAXX*3; x++) {
			if(map[y][x]=='X') printf("█");
			else if(map[y][x]=='.') printf(".");
			else printf(" ");
		}
		printf("\n");
	}
}

// Read input file line by line (e.g., into an array)
char **readInput() {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int main(int argc, char *argv[]) {

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

	int curr = 1;

	// Grow the distance matrix
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
		if(!change) break;
		curr++;
	}

	printDijk(dijk);
	printf("Furthest %d steps\n", curr-1);

	// Let's make an enlarged view
	char **three=calloc(MAXY*3,sizeof(char*));
	for(int iter=0; iter<MAXY*3; iter++) three[iter]=calloc(MAXX*3,sizeof(char));

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(dijk[y][x].dist) {
				three[y*3+1][x*3+1]='X';
				if(dijk[y][x].north) three[y*3][x*3+1]='X';
				if(dijk[y][x].south) three[y*3+2][x*3+1]='X';
				if(dijk[y][x].west) three[y*3+1][x*3]='X';
				if(dijk[y][x].east) three[y*3+1][x*3+2]='X';
			}
		}
	}
	for(y=0; y<MAXY*3; y++) {
		if(!three[y][0]) three[y][0]='.';
		if(!three[y][MAXX*3-1]) three[y][MAXX*3-1]='.';
	}
	for(x=0; x<MAXX*3; x++) {
		if(!three[0][x]) three[0][x]='.';
		if(!three[MAXY*3-1][x]) three[MAXY*3-1][x]='.';
	}

	while( 1 ) {
		int change=0;

		for(y=1; y<MAXY*3-1; y++) {
			for(x=1; x<MAXX*3-1; x++) {
				if(!three[y][x]) {
					if(three[y-1][x]=='.') { three[y][x]='.'; change = 1; }
					if(three[y+1][x]=='.') { three[y][x]='.'; change = 1; }
					if(three[y][x-1]=='.') { three[y][x]='.'; change = 1; }
					if(three[y][x+1]=='.') { three[y][x]='.'; change = 1; }
				}
			}
		}

		if(!change) break;
	}

	printThree(three);

	int count=0;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(three[y*3 + 1][x*3 + 1]==0) count ++;
		}
	}

	printf("Inside: %d\n", count);

	return 0;
}
