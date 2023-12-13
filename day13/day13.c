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
	for(y=0; map[y][0]; y++) {
		for(x=0; map[y][x]; x++) {
			printf("%c", map[y][x]);
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

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
	char ***map=calloc(MAXZ,sizeof(char**));
	for(int iter=0; iter<MAXZ; iter++) {
		map[iter]=calloc(MAXY,sizeof(char*));
		for(int ater=0; ater<MAXY; ater++) map[iter][ater]=calloc(MAXX,sizeof(char));
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

int checkMirror(char **map) {

	int retVal=0;
	int mx, my, x, y, d;

	for(mx=0; map[0][mx]; mx++);
	for(my=0; map[my][0]; my++);

	char **mirror=calloc(MAXY,sizeof(char*));
	for(int ater=0; ater<MAXY; ater++) mirror[ater]=calloc(MAXX,sizeof(char));

	for(y=0; y<my; y++) {
		for(x=0; x<mx; x++) {
			mirror[y][mx-x-1] = map[y][x];
		}
	}

	printf("\nMirror:\n");
	printMap(mirror);

	printf("Width (mx): %d, starting from 2-%d+%d=%d\n", mx, mx, mx%2,2-mx+mx%2);

	// Horizontal


	for(d=2-mx; d<=mx-2; d+=2) {
		int allfit=1;
		int lookingat=0;
		char **test=(char**)calloc(MAXY,sizeof(char*));
		for(int iter=0; iter<MAXY; iter++) test[iter]=(char*)calloc(MAXX,sizeof(char));
		for(y=0; y<my; y++)
			for(x=0;x<mx*3; x++) test[y][x]=' ';

		printf("\nShift %d:\n", d);
		printf("Checking between columns %d and %d:\n",x=(d>0)?d:0, (d>0?mx:mx+d));
		for(x=0; x<mx; x++) printf(" ");
		for(x=0; x<mx; x++) if((x>=(d>0?d:0))&&x<(d>0?mx:mx+d)) printf("_"); else printf(" ");
		printf("\n");
		for(x=0; x<mx; x++) if((x>=(d>0?d:0))&&x<(d>0?mx:mx+d)) lookingat++;
		for(y=0; y<my; y++) {
			for(x=(d>0)?d:0; x<(d>0?mx:mx+d); x++) {
				if(map[y][x]!=mirror[y][x-d]) {
					allfit=0;
				}
				else test[y][x+mx]='X';
			}
		}
		for(y=0; y<my; y++) {
			for(x=0; x<mx; x++) {
				if(test[y][x+mx]==' ') test[y][x+mx]=map[y][x];
				if(test[y][x+mx+d]==' ') test[y][x+mx+d]=mirror[y][x];
			}
		}
		printMap(test);

		if(allfit) {
			printf("Shift %d fits\n", d);
			if(d<0) return lookingat/2;
			else return lookingat/2+d;
		}
			
//		for(int iter=0; iter<MAXY; iter++) {
//			printf("So where %d? %ld\n", iter, test[iter]);
//			if(test[iter]) free(test[iter]);
//		}
//		free(test);
	}
	


	
	for(int ater=0; ater<MAXY; ater++) free(mirror[ater]);
	free(mirror);

	return retVal;
}

int main(int argc, char *argv[]) {

	int i=0, sum = 0;	
	char ***maps = readInput();
	int score;

	i=1;
//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; maps[i][0][0]; i++) {
		printMap(maps[i]);
		printf("%d\n\n",i);
		score=checkMirror(maps[i]);
		if(!score) {
			// Rotate
			int x=0, y=0, my, mx;
			char **rot=calloc(MAXY,sizeof(char*));
			for(int iter=0; iter<MAXY; iter++) rot[iter]=calloc(MAXX,sizeof(char));

			for(mx=0; maps[i][0][mx]; mx++);
			for(my=0; maps[i][my][0]; my++);

			printf("Original dimensions %dx%d, rotated dimenstions %dx%d\n", mx, my, my, mx);

			for(y=0; y<my; y++)
				for(x=0; x<mx; x++)
					rot[mx-x-1][y]=maps[i][y][x];

			printf("Trying rotation:\n");

			printMap(rot);

			score=100*checkMirror(rot);

			for(int iter=0; iter<MAXY; iter++) free(rot[iter]);
			free(rot);
			
		}

		sum+=score;

		assert(score);

	}

	printf("Sum: %d\n", sum);

	return 0;
}
