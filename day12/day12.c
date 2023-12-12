#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
//#define INPUT "input.txt"
#define MAXX 7
#define MAXY 1000
#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	char *map;
	int vars; // Possible variations based on ?
	int *ops;
} TLine;

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
TLine *readInput() {
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
	TLine *inst=(TLine*)calloc(MAXX, sizeof(TLine));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	// char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) continue;

		inst[count].ops=calloc(MAXX, sizeof(int));

		// Read tokens from single line
		char *token;
		inst[count].map = strdup(strtok(line, " "));
		int y=0;
		while( 1 ) {
			if(!(token = strtok(NULL, ","))) break;
			inst[count].ops[y++] = atoi(token);
		}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

void countVars(TLine *array) {
	for(int i=0; array[i].map; i++) {
		array[i].vars=1;
		for(int y=0; y<strlen(array[i].map); y++) {
			if(array[i].map[y]=='?') array[i].vars=array[i].vars << 1;
		}
	}
}

char *variation(int num, TLine line) {

	if(num>=line.vars) return NULL;

	char *map=strdup(line.map);
	int q=-1;
	for(int i=0; i<strlen(map); i++) {
		if(map[i]=='?') {
			q++;
			int bit=1<<q;
			if(num & bit) map[i]='#';
			else map[i]='.';
		}
	}

	return map;
}

int *sum(char *map) {

	int *sum = calloc(MAXX, sizeof(int));
	int inq = 0;
	int y = -1;

	for(int i=0; i<strlen(map); i++) {
		if(map[i]=='#') {
			if(inq) sum[y]++;
			else {
				sum[++y]=1;
				inq=1;
			}
		}
		else inq=0;
	}

	return sum;
}

int compare(int *a, int *b) {

	int la, lb;
	for(la=0; a[la]; la++);
	for(lb=0; b[lb]; lb++);

	if(la!=lb) return 0;

	for(int y=0; y<=la; y++) {
		if(a[y]!=b[y]) return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {

	TLine *array = readInput();
	countVars(array);
	int i=0;	
//	array = readInput();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; array[i].map; i++) {
		printf("%s\t", array[i].map);
		for(int y=0; array[i].ops[y]; y++) printf("%d, ", array[i].ops[y]);
		printf("\t%d variations\n", array[i].vars);
	}

	int total=0;

	for(int line=0; array[line].map; line++) {
		i=0;
		while (1) {

			char *v=variation(i++, array[line]);
			if(!v) break;


			int *s=sum(v);
			if(compare(s, array[line].ops)) {
				printf("l%04d:\t%3d: %s\t", line, i, v);
				for(int y=0; s[y]; y++) printf("%d, ", s[y]);
				total++;
				printf(" match %d!\n", total);
			}


			free(v);
			free(s);
		}
	}

	return 0;
}
