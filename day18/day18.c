#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
//#define INPUT "input.txt"
#define MAXOP 800
//#define MAXX 76
//#define MAXY 26
#define INPUT "unit1.txt"
#define MAXX 80
#define MAXY 80

// Point structure definition
typedef struct {
	char dir;
	int dx;
	int dy;
	int count;
	char *color;
} TOp;

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
			if( map[y][x]) 	printf("%c", map[y][x]);
			else printf(" ");
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
TOp *readInput() {
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
	TOp *inst=(TOp*)calloc(MAXOP, sizeof(TOp));

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
		inst[count].color=calloc(10, sizeof(char));
		sscanf(line,"%c %d %s",
			&(inst[count].dir),
			&(inst[count].count),
			inst[count].color);

		switch(inst[count].dir) {
		case 'R':
			inst[count].dx=1;
			inst[count].dy=0;
			break;
		case 'L':
			inst[count].dx=-1;
			inst[count].dy=0;
			break;
		case 'U':
			inst[count].dx=0;
			inst[count].dy=-1;
			break;
		case 'D':
			inst[count].dx=0;
			inst[count].dy=1;
			break;
		default:
			assert(0);
			break;
		}

		// Read tokens from single line
		//char *token;
		//token = strtok(line, ",");
		//while( 1 ) {
		//	if(!(token = strtok(NULL, ","))) break;
		//}

		count++;
	}

	fclose(input);

//	printMap(map);

	return inst;
//	return map;
}

int main(int argc, char *argv[]) {

	int i=0;	
	TOp *array = readInput();
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	int x = MAXX / 2;
	int y = MAXY / 2;
	for(i=0; array[i].dir; i++) {
		printf("%c %d %s\n", array[i].dir, array[i].count, array[i].color);
		for(int q=0; q<array[i].count; q++) {
			x+=array[i].dx;
			y+=array[i].dy;
			map[y][x]='#';
		}
	}
	printMap(map);

	return 0;
}
