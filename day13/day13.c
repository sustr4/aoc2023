#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 40
#define MAXY 40
#define MAXZ 101
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

char **allocMap() {
	char **retVal=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) retVal[iter]=calloc(MAXX,sizeof(char));
	return retVal;
}

void freeMap(char **map) {
	for(int iter=0; iter<MAXY; iter++) if(map[iter]) free(map[iter]);
	free(map);
}

// Print a two-dimensional array
void printMap (char **map) {
	int x,y;
	for(y=0; map[y][0]; y++) {
		for(x=0; map[y][x]; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}

// Read input file line by line (e.g., into an array)
char ***readInput() {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int mapno = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a three-dimensional arrray of chars
	int x=0, y=0;
	char ***map=calloc(MAXZ,sizeof(char**));
	for(int iter=0; iter<MAXZ; iter++) {
		map[iter]=allocMap();
	}

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) {
			mapno++;
			y=0;
			continue;
		}

		// Read into map
		for(x=0; x<strlen(line); x++) map[mapno][y][x] = line[x];
		y++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int checkMirror(char **map, int ignorescore) {

	int retVal=0;
	int mx, my, x, y, d;

	for(mx=0; map[0][mx]; mx++);
	for(my=0; map[my][0]; my++);

	char **mirror=allocMap();

	for(y=0; y<my; y++) 
		for(x=0; x<mx; x++) 
			mirror[y][mx-x-1] = map[y][x];

	for(d=2-mx; d<=mx-2; d+=2) {
		int allfit=1;
		int lookingat=0;

		for(x=0; x<mx; x++) if((x>=(d>0?d:0))&&x<(d>0?mx:mx+d)) lookingat++;
		for(y=0; y<my; y++) {
			for(x=(d>0)?d:0; x<(d>0?mx:mx+d); x++) {
				if(map[y][x]!=mirror[y][x-d]) {
					allfit=0;
					break;
				}
			}
		}


		if(allfit) {
			if(d<0) retVal = lookingat/2;
			else retVal = lookingat/2+d;

			if(retVal!=ignorescore) break;
			else retVal=0;
		}
	}

	freeMap(mirror);
	return retVal;
}

int scoreMap(char **map, int ignorescore) {

	int retVal=checkMirror(map, ignorescore);
	if(!retVal) {
		// Rotate
		int x=0, y=0, my, mx;
		char **rot=allocMap();

		for(mx=0; map[0][mx]; mx++);
		for(my=0; map[my][0]; my++);

		for(y=0; y<my; y++)
			for(x=0; x<mx; x++)
				rot[mx-x-1][y]=map[y][x];

		retVal=100*checkMirror(rot, ignorescore/100);

		freeMap(rot);
	}

	return retVal;
}

int main(int argc, char *argv[]) {

	int i=0, sum = 0, newsum = 0;	
	char ***maps = readInput();
	int score;

	i=1;
//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; maps[i][0][0]; i++) {
//		printf("Start\n");
//		printMap(maps[i]);

		char **map=maps[i];

		int oldscore=scoreMap(map, 0);

		score = 0;
		for(int y=0; map[y][0]; y++) {
			for(int x=0; map[0][x]; x++) {
				char oldchar=map[y][x];
				switch (oldchar) {
					case '.':
						map[y][x]='#';
						break;
					case '#':
						map[y][x]='.';
						break;
				}

				score = scoreMap(map, oldscore);
				if((score!=0) && (score!=oldscore)) goto found;

				map[y][x]=oldchar;
			}
		}

found:

		sum+=oldscore;
		newsum+=score;
		printf("%d:\t%4d (%d)\t%5d (%d)\n",i, oldscore, sum, score, newsum);


		assert(score);

	}

	printf("Old sum: %d\n", sum);
	printf("New sum: %d\n", newsum);

	return 0;
}
