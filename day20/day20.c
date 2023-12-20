#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
//#define INPUT "input.txt"
#define MAXX 76
#define MAXY 70
#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10


int modNo(char *name) {
	static char **names=NULL;

	if(!names) names=calloc(1000, sizeof(char*));

	int i;
	for(i=0; names[i]; i++) {
		if(!strcmp(names[i],name)) return i;
	}
	names[i]=strdup(name);
	return i;
}

// Point structure definition
typedef struct {
	char *name;
	char type;
	int *connect;
} TModule;

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
TModule *readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0,no;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	TModule *inst=(TModule*)calloc(MAXY, sizeof(TModule));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

		// Read tokens from single line
		modNo("NULL");
		char *token;
		token = strtok(line, " ");
		char *tempName=strdup(token+(token[0]=='b'?0:1));
		no=modNo(tempName);
		inst[no].name=tempName;
		inst[no].type=token[0];
		token = strtok(NULL, " "); // this gets rid of the ->
		inst[no].connect=calloc(10,sizeof(int));
		int j=0;
		while( 1 ) {
			if(!(token = strtok(NULL, ", "))) break;
			inst[no].connect[j]=modNo(token);
			j++;
		}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

int main(int argc, char *argv[]) {

	int i=0;	
//	array = readInput();
	TModule *module = readInput();

	for(i=1; module[i].name; i++) {
		printf("Module %d (%s), type %c, connects to ", i, module[i].name, module[i].type);
		for(int q=0; module[i].connect[q]; q++) {
			printf("%d (%s), ", module[i].connect[q], module[module[i].connect[q]].name);
		}
		printf("\n");
	}


//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	return 0;
}
