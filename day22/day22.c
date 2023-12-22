#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 76
#define MAXY 26
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

#define MAXBR 1500

// Brick structure definition
typedef struct {
	int *c;
	int **fills;
} TBrick;

void printBrick(int b, TBrick *brick) {
	for(int j=0; brick[b].fills[j]; j++) printf("[%d,%d,%d], ", brick[b].fills[j][0], brick[b].fills[j][1], brick[b].fills[j][2]);
}

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
TBrick *readInput() {
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
	TBrick *inst=(TBrick*)calloc(MAXBR, sizeof(TBrick));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	// char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into array
		inst[count].c=calloc(6,sizeof(int));
		sscanf(line,"%d,%d,%d~%d,%d,%d",
			&(inst[count].c[0]),
			&(inst[count].c[1]),
			&(inst[count].c[2]),
			&(inst[count].c[3]),
			&(inst[count].c[4]),
			&(inst[count].c[5]));

		int dir=0;
		int len=0;
		for(int i = 0; i<3; i++)
			if (inst[count].c[i]!=inst[count].c[i+3]) {
				if(inst[count].c[i+3] < inst[count].c[i]) { // Swap values if they run the other way
					int temp = inst[count].c[i+3];
					inst[count].c[i+3] = inst[count].c[i];
					inst[count].c[i] = temp;
				}
				dir=i;
				len=inst[count].c[i+3] - inst[count].c[i];
			}
		assert(len>=0);
		inst[count].fills=calloc(len+2, sizeof(int*));
		for(int i = 0; i<=len; i++) {
			inst[count].fills[i]=calloc(3, sizeof(int));
			for(int j = 0; j<3; j++) inst[count].fills[i][j]=inst[count].c[j];
			inst[count].fills[i][dir]+=i;
		}	

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

int canfall(int b, TBrick *brick) {

	if(!brick[b].fills) return 0;
	for(int j=0; brick[b].fills[j]; j++) if(brick[b].fills[j][2]<=1) return 0;

	int i;
	for(i=0; brick[i].c; i++) {
		if(i==b) continue;
		if(!brick[i].fills) continue;
		for(int j=0; brick[i].fills[j]; j++) {
			for(int y=0; brick[b].fills[y]; y++) {
				if((brick[i].fills[j][0] == brick[b].fills[y][0]) &&
				   (brick[i].fills[j][1] == brick[b].fills[y][1]) &&
				   (brick[i].fills[j][2] == brick[b].fills[y][2]-1)) return 0;
			}
		}
	}
	return 1;
}

void fall(int b, TBrick *brick) {
	printf("Brick %d falling from ", b);
	printBrick(b, brick);
	for(int j=0; brick[b].fills[j]; j++) brick[b].fills[j][2]--;
	printf(" to ");
	printBrick(b, brick);
	printf("\n");
}

int main(int argc, char *argv[]) {

	int i=0;	
	TBrick *brick = readInput();

	// Let all fall
	int change;
	while(1) {
		change=0;
		for(i=0; brick[i].c; i++) {
			if(canfall(i, brick)) {
				fall(i, brick);
				change=1;
			}
		}
		if(!change) break;
	}

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; brick[i].c; i++) {
		printf("Brick %d", i);
//		for(int j=0; j<6; j++) printf("%3d %c ", brick[i].c[j], j==2?'~':' '); 
		printf("\n\tFills: ");
		printBrick(i, brick);
		if(canfall(i, brick)) printf("\n\tCan fall");
		printf("\n");
	}

	int sum = 0;

	for(i=0; brick[i].c; i++) {
		int safe = 1;

		int **remove;
		remove=brick[i].fills;
		brick[i].fills=NULL;

		for(int b=0; brick[b].c; b++) {
			if(b == i) continue;
			if(canfall(b, brick)) {
				printf("Brick %d will fall if %d is removed\n", b, i);
				safe = 0;
				break;
			}
		}
		if(safe) sum++;
		brick[i].fills=remove;
	}

	printf("%d bricks can be safely removed\n", sum);

	return 0;
}
