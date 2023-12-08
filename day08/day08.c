#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 17577
#define MAXY 26
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	char *name;
	char *lname;
	char *rname;
	int left;
	int right;
} TTurn;

TTurn *node;
char *instructions=0;
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

int encode(char *node) {
	int i,retval=1,offset;
	for(i=0;i<3;i++) {
		if(i==2) offset=1;
		else if(i==1) offset='Z'-'A'+1;
		else if(i==0) offset=('Z'-'A'+1)*('Z'-'A'+1);
		retval+=(node[i]-'A')*offset;
	}
	return retval;
}

// Read input file line by line (e.g., into an array)
TTurn *readInput() {
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


	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) continue;

		if(!instructions) {
			instructions = strdup(line);
			continue;
		}

		// Read into array
		char *point = calloc(4,sizeof(char));
		char *left = calloc(4,sizeof(char));
		char *right = calloc(4,sizeof(char));
		strncpy(point,line,3);
		strncpy(left,line+7,3);
		strncpy(right,line+12,3);
		int id = encode(point);
		node[id].name=point;
		node[id].lname=left;
		node[id].rname=right;
		node[id].left=encode(left);
		node[id].right=encode(right);

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

	return node;
//	return map;
}

int main(int argc, char *argv[]) {

//	TTurn *array;
//	int i=0;	
//	array = readInput();
	node=(TTurn*)calloc(MAXX, sizeof(TTurn));

	node = readInput();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	int i = encode("AAA");
	int end = encode("ZZZ");
	int y = 0;
	int count = 0;
	while(i!=end) {

		if(node[i].left) printf("%s (%d): (%s, %s), (%d %d) -> %c\n", node[i].name, i, node[i].lname, node[i].rname, node[i].left, node[i].right, instructions[y]);

		if(instructions[y]=='L') i=node[i].left;
		else if(instructions[y]=='R') i=node[i].right;
		count ++;

		y++;
		if((y>=strlen(instructions)) || ((instructions[y]!='L') && (instructions[y]!='R'))) y=0;
	}

	printf("%d\n", count);

	return 0;
}
