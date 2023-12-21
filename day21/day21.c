#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 131
#define MAXY 131
#define GOAL 196
//#define INPUT "unit1.txt"
//#define MAXX 11
//#define MAXY 11
//#define GOAL 6

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
void printDist (int **dist) {
	int x,y;
	for(y=0; y<MAXY*3; y++) {
		for(x=0; x<MAXX*3; x++) {
			if(dist[y][x])	printf("%d", dist[y][x]%10);
			else printf(" ");
		}
		printf("\n");
	}
}

void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY*3; y++) {
		for(x=0; x<MAXX*3; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █ and border elements ┃━┗┛┏┓
// Color printf("\033[1;31mR \033[1;32mG \033[1;34mB \033[0moff\n");

// Retrieve nth neighbor from a map
int dy[] = { -1,  0, 0, 1 };
int dx[] = {  0, -1, 1, 0 };
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<4));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY*3) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX*3)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
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
	char **map=calloc(MAXY*3,sizeof(char*));
	for(int iter=0; iter<MAXY*3; iter++) map[iter]=calloc(MAXX*3,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		for(x=0; x<MAXX; x++) {
			for(int a=0; a<3; a++) {
				for(int b=0; b<3; b++) {
					map[y+a*MAXY][x+b*MAXY] = line[x];
				}
			}
		}
		y++;
	}

	fclose(input);
	if (line)
	free(line);

	printMap(map);

	return map;
}

int cnt(int **dist) {
	int ret=0;

	for(int y=0; y<MAXY*3; y++) {
		for(int x=0; x<MAXX*3; x++) {
			if((dist[y][x]) && (dist[y][x]<=GOAL+1)) ret ++;
		}
	}
	return ret;
}

int main(int argc, char *argv[]) {

	char **map=readInput();
	int **dist=calloc(MAXY*3,sizeof(int*));
	for(int iter=0; iter<MAXY*3; iter++) dist[iter]=calloc(MAXX*3,sizeof(int));

	int starts=0;
	for(int y=0; y<MAXY*3; y++)
		for(int x=0; x<MAXX*3; x++)
			if(map[y][x]=='S') {
				map[y][x]='.';
				if(++starts==5) dist[y][x]=1;
			}

	int s;
	for(s=1; s<=GOAL; s++) {
		for(int y=0; y<MAXY*3; y++) {
			for(int x=0; x<MAXX*3; x++) {
				if(dist[y][x]!=s) continue;
//				printf("[%d][%d]: %d!=%d\n", y, x, dist[y][x], s);
				for(int n=0; n<4; n++) {
					char nb = mapnb(map, y, x, n);
//					printf("\t nb %d: %c\n", n, nb);
					if((nb =='.') &&
						(dist[y+dy[n]][x+dx[n]]==0)) dist[y+dy[n]][x+dx[n]]=s+1;
				}
				
			}
		}
		for(int y=0; y<MAXY*3; y++) 
			for(int x=0; x<MAXX*3; x++) if(dist[y][x]<=s) dist[y][x]=0;
	}

	printDist(dist);

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}


	printf("Reached %d tiles.\n", cnt(dist));
	printf("26501365 is like %d. Go %d\n", 26501365 % MAXX, 26501365 % MAXX + MAXX);

	return 0;
}
