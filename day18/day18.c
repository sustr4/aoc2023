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
	long count2;
} TOp;
typedef struct {
	long x;
	long y;
} TPoint;


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

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		inst[count].dir=token[0];
		token = strtok(NULL, " ");
		inst[count].count=atoi(token);
		token = strtok(NULL, " ");
		inst[count].color = strdup(token);

		printf("%s\n", inst[count].color);

		inst[count].dir2=inst[count].color[7];
		switch(inst[count].dir2) {
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

	return inst;
}

int main(int argc, char *argv[]) {

	int i=0;	
	TOp *array = readInput();
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	TPoint *polygon=(TPoint*)calloc(MAXOP, sizeof(TPoint));

	int x = 0;
	int y = 0;
	int pt=0;
	polygon[pt].x=x;
	polygon[pt].y=y;
	long halves=0;
	for(i=0; array[i].dir; i++) {
		printf("%c %d %s T2: %c %ld\n", array[i].dir, array[i].count, array[i].color, array[i].dir2, array[i].count2);
		x+=array[i].count2*array[i].dx;
		y+=array[i].count2*array[i].dy;
		pt++;
		polygon[pt].x=x;
		polygon[pt].y=y;
		halves+=array[i].count2;
	}

	long a=0;
	long b=0;

	for(int p=pt; p>0; p--) {
		printf("(%9ld,%9ld)\t", polygon[p].x, polygon[p].y);
		a+=polygon[p].x*polygon[p-1].y;
		printf("%9ld x %9ld\t", polygon[p].x, polygon[p-1].y);
		b+=polygon[p].y*polygon[p-1].x;
		printf("%9ld x %9ld\n", polygon[p].y, polygon[p-1].x);
	}
	printf("(%9ld,%9ld)\n", polygon[0].x, polygon[0].y);

	printf("A = %ld, B = %ld, A-B = %ld, half = %ld, + %ld halves = %ld\n", a, b, a-b, (a-b)/2, halves,  labs((a-b)/2) + halves/2 + 1);

	return 0;
}
