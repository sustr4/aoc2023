#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary definitions, set as required
#define MAXX 140
#define MAXY 140
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
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(map[y][x]) printf("%c", map[y][x]);
			else printf("_");
		}
		printf("\n");
	}
}
// Full block character for maps █


// Read input file line by line (e.g., into an array)
char **readInput() {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen("input.txt", "r");
//	input = fopen("unit1.txt", "r");
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


//	return inst;
	return map;
}

int isnum(char c) {
	return ((c>='0') && (c<='9'));
}

int main(int argc, char *argv[]) {

	int x=0, y=0, dx, dy;
	char **clean=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) clean[iter]=calloc(MAXX,sizeof(char));
	int change = 0;

	char **map=readInput();

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXY; x++) {
			if((map[y][x]!='.') &&
			    ((map[y][x]<'0') || (map[y][x]>'9'))) { // Symbol, all numbers around it count
				for(dy=-1; dy<=1; dy++) {
					for(dx=-1; dx<=1; dx++) {
						if(y+dy<0) break;
						if(x+dx<0) break;
						if(x+dx>=MAXX) break;
						if(y+dy>=MAXY) break;
						if((map[y+dy][x+dx]>='0')&&(map[y+dy][x+dx]<='9')) clean[y+dy][x+dx]=map[y+dy][x+dx];
					}
				}
			}
		}
	}

	while(1) {
		change=0;
		for(y=0; y<MAXY; y++) {
			for(x=0; x<MAXY; x++) {
				if(isnum(clean[y][x])) {
					if(x>0) {
						if(isnum(clean[y][x]) && 
						   !isnum(clean[y][x-1]) &&
						   isnum(map[y][x-1])) {
							clean[y][x-1]=map[y][x-1];
							change++;
//							printf("- %d,%d,%c\n",y,x,map[y][x+1]);
						}
					}
					if(x<MAXX-1) {
						if(isnum(clean[y][x]) && 
						   !isnum(clean[y][x+1]) &&
						   isnum(map[y][x+1])) {
							clean[y][x+1]=map[y][x+1];
							change++; 
//							printf("+ %d,%d,%c\n",y,x,map[y][x+1]);
						}
					}

				}
			}
		}	
		if(!change) break;
	}

	printMap(clean);

	return 0;
}
