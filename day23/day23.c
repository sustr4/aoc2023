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
//#define MAXX 23
//#define MAXY 23

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
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █ and border elements ┃━┗┛┏┓
// Color printf("\033[1;31mR \033[1;32mG \033[1;34mB \033[0moff\n");

void printDist (int **dist, char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(map[y][x]=='#') {
				printf("\033[0;31m##\033[0m");
				continue;
			}
			if(dist[y][x]>0) printf("\033[1;34m");
			printf("%2d", dist[y][x]%100);
			printf("\033[0m");
		}
		printf("\n");
	}
}
// Retrieve nth neighbor from a map
int dy[] = { -1,  0, 0,  1 };
int dx[] = {  0, -1, 1,  0 };
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<4));
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
	int **dist=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) dist[iter]=calloc(MAXX,sizeof(int));

	for(x=0; x<MAXX; x++) if(map[0][x]=='.') dist[0][x]=1;

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	int s = 1;
	while(1) {
		int change=0;

		for(y=0; y<MAXY; y++) {
			for(x=0; x<MAXX; x++) {
				if(dist[y][x]!=s) continue;
				for(int n = 0; n<4; n++) {
					char nb = mapnb(map, y, x, n);
					if(!nb) continue;
					switch(map[y+dy[n]][x+dx[n]]) {
					case '#':
						continue;
						break; // Yea, I know
					case '>':
						if(dx[n]!=1) break;
						if((dist[y+dy[n]][x+dx[n]]==0) ||
						   (dist[y+dy[n]][x+dx[n]]<s-1)) dist[y+dy[n]][x+dx[n]] = s+1;
						change = 1;
						break;
					case 'v':
						if(dy[n]!=1) break;
						if((dist[y+dy[n]][x+dx[n]]==0) ||
						   (dist[y+dy[n]][x+dx[n]]<s-1)) dist[y+dy[n]][x+dx[n]] = s+1;
						change = 1;
						break;
					case '.':
						if((dist[y+dy[n]][x+dx[n]]==0) ||
						   (dist[y+dy[n]][x+dx[n]]<s-1)) dist[y+dy[n]][x+dx[n]] = s+1;
						change = 1;
						break;
					}
				}
			}
		}
		printf("\n");
		printDist(dist, map);
		if(!change) break;
		s++;
	}


	for(x=0; x<MAXX; x++) if(map[MAXY-1][x]=='.') printf("Last field [%d,%d]: %d. Search ended after %d steps\n", x, MAXY-1, dist[MAXY-1][x]-1, s);
	return 0;
}
