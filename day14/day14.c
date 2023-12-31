#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>
#include<time.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 100
#define MAXY 100
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10


// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

// Print a two-dimensional array
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		printf("%3d ", MAXY-y);
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
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

	printMap(map);

	return map;
}

int north(char **map) {

	for(int y=1; y<MAXY; y++) 
		for(int x=0; x<MAXX; x++) 
			if((map[y][x]=='O')) {
				int v=y;
				while((v>0)&&(map[v-1][x]=='.')) {
					map[v-1][x]='O';
					map[v][x]='.';
					v--;
				}
			}

	return 0;
}
int south(char **map) {

	for(int y=MAXY-2; y>=0; y--) 
		for(int x=0; x<MAXX; x++) 
			if((map[y][x]=='O')) {
				int v=y;
				while((v<MAXY-1)&&(map[v+1][x]=='.')) {
					map[v+1][x]='O';
					map[v][x]='.';
					v++;
				}
			}

	return 0;
}

int west(char **map) {

	for(int x=1; x<MAXX; x++) 
		for(int y=0; y<MAXY; y++) 
			if((map[y][x]=='O')) {
				int v=x;
				while((v>0)&&(map[y][v-1]=='.')) {
					map[y][v-1]='O';
					map[y][v]='.';
					v--;
				}
			}

	return 0;
}

int east(char **map) {

	for(int x=MAXX-2; x>=0; x--) 
		for(int y=0; y<MAXY; y++) 
			if((map[y][x]=='O')) {
				int v=x;
				while((v<MAXY-1)&&(map[y][v+1]=='.')) {
					map[y][v+1]='O';
					map[y][v]='.';
					v++;
				}
			}

	return 0;
}

int load(char **map) {

	int load=0;
	for(int y=0; y<MAXY; y++)
		for(int x=0; x<MAXX; x++)
			if(map[y][x]=='O') load+=MAXY-y;

	return load;
}

int countO(char **map) {

	int O=0;
	for(int y=0; y<MAXY-1; y++)
		for(int x=0; x<MAXX; x++)
			O++;

	return O;

}

int main(int argc, char *argv[]) {

	char **map=readInput();
	printf("No of Os: %d\n", countO(map));


	north(map);
	printMap(map);
	printf("No of Os: %d\n", countO(map));
	printf("Load is %d\n", load(map));



	for(int m=0; m<1000; m++) {
		for(int i=0; i< 1000000; i++) {
			north(map); west(map); south(map); east(map);
		}
		time_t rawtime;
		struct tm * timeinfo;

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		printf("%d million, load %d\t", m, load(map));
		printf ("%s", asctime (timeinfo) );
	}

	printf("\n");
	printMap(map);
	printf("No of Os: %d\n", countO(map));


	printf("Load is %d\n", load(map));

	return 0;
}
