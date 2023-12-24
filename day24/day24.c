#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>
#include<math.h>

// Boundary and input file definitions, set as required
/*#define INPUT "input.txt"
#define MAXX 301
#define MAXY 301
#define LOW 200000000000000
#define HIGH 400000000000000 /**/
#define INPUT "unit1.txt"
#define MAXX 5
#define MAXY 5
#define LOW 7
#define HIGH 27 /**/

// Point structure definition
typedef struct {
	long *p;
	long *v;
} TStone;

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
TStone *readInput() {
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
	TStone *inst=(TStone*)calloc(MAXX, sizeof(TStone));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	// char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		inst[count].p=calloc(3, sizeof(long));
		inst[count].v=calloc(3, sizeof(long));
		sscanf(line,"%ld, %ld, %ld @ %ld, %ld, %ld",
			&(inst[count].p[0]),
			&(inst[count].p[1]),
			&(inst[count].p[2]),
			&(inst[count].v[0]),
			&(inst[count].v[1]),
			&(inst[count].v[2]));

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

	return inst;
}

void printStone(int i, TStone *stone) {
	printf("%d: ", i);
	for(int q=0; q<3; q++) printf("%ld, ", stone[i].p[q]);
	printf(" (");
	for(int q=0; q<3; q++) printf("%ld, ", stone[i].v[q]);
	printf(")\n");
}

int main(int argc, char *argv[]) {

	int i=0;	
	TStone *stone = readInput();
	readInput();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; i<MAXX-1; i++) {
		printStone(i, stone);
		for(int ii=i+1; ii<MAXX; ii++) {
			printf("On "); printStone(ii, stone);

			double intersection[3];

			double x = (double)stone[i].p[0];
			double a = (double)stone[i].v[0];
			double y = (double)stone[ii].p[0];
			double b = (double)stone[ii].v[0];
			double o = (double)stone[i].p[1];
			double c = (double)stone[i].v[1];
			double p = (double)stone[ii].p[1];
			double d = (double)stone[ii].v[1];

			double q = (o*a + c*y - c*x - p*a) / (d*a - c*b);

			intersection[0] = y + q*b;
			intersection[1] = p + q*d;

			printf("Intersect: [%f, %f]\n", intersection[0], intersection[1]);
			
		}
	}

	return 0;
}
