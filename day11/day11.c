#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
//#define INPUT "input.txt"
//#define MAXX 140
//#define MAXY 140
#define INPUT "unit1.txt"
#define MAXX 10
#define MAXY 10

// Point structure definition
typedef struct {
	int x;
	int y;
} TGalaxy;

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
	// TGalaxy *inst=(TGalaxy*)calloc(MAXX, sizeof(TGalaxy));

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

	printMap(map);

	return map;
}

int countGalaxies(char **map) {
	int retval=0;
	for(int y=0; y<MAXY; y++) {
		for(int x=0; x<MAXY; x++) {
			if(map[y][x]=='#') retval++;
		}
	}
	return retval;
}

void mapGalaxies(char **map, TGalaxy *gal) {
	int i=0;
	for(int y=0; y<MAXY; y++) {
		for(int x=0; x<MAXY; x++) {
			if(map[y][x]=='#') {
				gal[i].y=y;
				gal[i].x=x;
				i++;
			}
		}
	}
}

void expand(TGalaxy *gal, int count) {
	int *len=calloc(MAXX, sizeof(int));
	int *hei=calloc(MAXY, sizeof(int));
	int i;

	for(i=0; i<MAXX; i++) len[i]=1; //Assume all exapnd
	for(i=0; i<MAXY; i++) hei[i]=1;
	
	for(i=0; i<count; i++) {
		len[gal[i].x] = 0; // Don't exapnd those that have galaxies
		hei[gal[i].y] = 0;
	}

	for(i=1; i<MAXX; i++) len[i]=len[i]+len[i-1]; // Sum up the expansions
	for(i=1; i<MAXX; i++) hei[i]=hei[i]+hei[i-1];
		
	for(i=0; i<MAXX; i++) printf("%d", len[i]);
	printf("\n");

	for(i=0; i<count; i++) {
		gal[i].x+=len[gal[i].x];
		gal[i].y+=hei[gal[i].y];
	}
	
}

long dist(TGalaxy *gal, int count) {
	int i,j;
	long sum = 0;

	for(i=0; i<count; i++) {
		for(j=i+1; j<count; j++) {
			sum+=abs(gal[i].x-gal[j].x) + abs(gal[i].y - gal[j].y);
		}
	}
	return sum;
}

int main(int argc, char *argv[]) {

//	TGalaxy *array;
//	int i=0;	
//	array = readInput();

	char **map=readInput();
	int count = countGalaxies(map);

	TGalaxy *gal=calloc(count+1, sizeof(TGalaxy));
	
	mapGalaxies(map, gal);

	expand(gal, count);

	for(int i=0; i<count; i++) printf("%2d: [%d,%d]\n", i, gal[i].x, gal[i].y);	

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	printf("Sum: %ld\n", dist(gal, count));

	return 0;
}
