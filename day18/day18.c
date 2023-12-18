#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
//#define INPUT "unit1.txt"
//#define INPUT "unit2.txt"
#define MAXOP 800
#define MAXX 1000
#define MAXY 1000

// Point structure definition
typedef struct {
	char dir;
	int dx;
	int dy;
	int count;
	char *color;
	char dir2;
	int count2;
} TOp;
typedef struct {
	int x;
	int y;
} TPoint;

int ymin=0, ymax=0, xmin=0, xmax=0;

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
	for(y=ymin; y<=ymax; y++) {
		for(x=xmin; x<=xmax; x++) {
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

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		inst[count].dir=token[0];
		token = strtok(NULL, " ");
		inst[count].count=atoi(token);
		token = strtok(NULL, " ");
		inst[count].color = strdup(token);

		printf("%s\n", inst[count].color);

		switch(inst[count].dir) {
		case '0':
		case 'R':
			inst[count].dx=1;
			inst[count].dy=0;
			break;
		case '2':
		case 'L':
			inst[count].dx=-1;
			inst[count].dy=0;
			break;
		case '3':
		case 'U':
			inst[count].dx=0;
			inst[count].dy=-1;
			break;
		case '1':
		case 'D':
			inst[count].dx=0;
			inst[count].dy=1;
			break;
		default:
			assert(0);
			break;
		}

		inst[count].dir2=inst[count].color[7];
		inst[count].color[7]=0;
		inst[count].count2=strtol(inst[count].color+2, NULL, 16);


		count++;
	}

	fclose(input);

//	printMap(map);

	return inst;
//	return map;
}

void colorIn (char **map) {
	int y, x;
	map[xmin][ymin]='.';
	while(1) {
		int change=0;

		for(y=0; y<MAXY; y++) {
			for(x=0; x<=MAXX; x++) {
				if(map[y][x]!=0) continue;
				for(int n=0; n<8; n++) {
					if(mapnb(map, y, x, n)=='.') {
						map[y][x]='.';
						change=1;
					}
				}
			}
		}
		if(!change) break;
	}
		for(y=ymin; y<=ymax; y++) {
			for(x=xmin; x<=xmax; x++) {
				if(!map[y][x]) map[y][x]='#';
			}
		}
}

int main(int argc, char *argv[]) {

	int i=0;	
	TOp *array = readInput();
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	TPoint *polygon=(TPoint*)calloc(MAXOP, sizeof(TPoint));

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	int x = 0;
	int y = 0;
	xmin=x; xmax=x;
	ymin=y; ymax=y;
	int pt=0;
	polygon[pt].x=x;
	polygon[pt].y=y;
	int halves=0;
	for(i=0; array[i].dir; i++) {
		printf("%c %d %s T2: %c %d\n", array[i].dir, array[i].count, array[i].color, array[i].dir2, array[i].count2);
		x+=array[i].count*array[i].dx;
		y+=array[i].count*array[i].dy;
		if(ymin>y) ymin=y;
		if(ymax<y) ymax=y;
		if(xmin>x) xmin=x;
		if(xmax<x) xmax=x;
		pt++;
		polygon[pt].x=x;
		polygon[pt].y=y;
		halves+=array[i].count;
	}

	printf("Dimensions: %d -- %d horizontally, %d -- %d vertically\n", xmin, xmax, ymin, ymax);

//	TPoint polygon[]={{0, 0}, {3,0}, {3,3}, {0,3}, {0,0}};
//	pt=4;

	int a=0;
	int b=0;

	for(int p=pt; p>0; p--) {
		printf("(%3d,%3d)\t", polygon[p].x, polygon[p].y);
		a+=polygon[p].x*polygon[p-1].y;
		printf("%3d x %3d\t", polygon[p].x, polygon[p-1].y);
		b+=polygon[p].y*polygon[p-1].x;
		printf("%3d x %3d\n", polygon[p].y, polygon[p-1].x);
	}
	printf("(%3d,%3d)\n", polygon[0].x, polygon[0].y);

	printf("A = %d, B = %d, A-B = %d, half = %d, + %d halves = %d\n", a, b, a-b, (a-b)/2, halves,  abs((a-b)/2) + halves/2 + 1);

//	printMap(map);
//	colorIn(map);
//	printMap(map);

	return 0;
}
