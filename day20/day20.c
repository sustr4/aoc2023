#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 100
#define MAXY 100
#define QLEN 10000
//#define INPUT "unit2.txt"
//#define MAXX 10
//#define MAXY 10

long pulses[3]={0,0,0};

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

typedef struct {
	int val;
	int from;
} TPulse;

// Point structure definition
typedef struct {
	char *name;
	char type;
	int state;
	TPulse *in;
	int idxtop;
	int idxbottom;
	int *memory;
	int *connect;
} TModule;

void push(int m, TModule *module, TPulse pulse) {

	if(!module[m].in) {
		module[m].in=calloc(QLEN, sizeof(TPulse));
		module[m].idxtop=0;
		module[m].idxbottom=0;
	}

	printf("\t pushing %d from %d (%s) into the queue for %d (%s)\n", pulse.val, pulse.from, module[pulse.from].name, m, module[m].name);

	pulses[pulse.val]++; // Stats

	module[m].in[module[m].idxtop++]=pulse;
	if(module[m].idxtop>=QLEN) module[m].idxtop=0;
	assert(module[m].idxtop!=module[m].idxbottom);
}

int hasInput(int m, TModule *module) {
	if(module[m].idxtop!=module[m].idxbottom) return 1;
	return 0;
}

TPulse pull(int m, TModule *module) {
	TPulse ret;
	assert(module[m].idxtop!=module[m].idxbottom);
	ret=module[m].in[module[m].idxbottom];
	module[m].idxbottom++;
	if(module[m].idxbottom>=QLEN) module[m].idxbottom=0;

	return ret;
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
TModule *readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0,no;

	modNo("button");
	assert((input = fopen(INPUT, "r")));
	while ((read = getline(&line, &len, input)) != -1) {
		char *token;
		token = strtok(line, " ");
		char *tempName=strdup(token+(token[0]=='b'?0:1));
		modNo(tempName);
	}
	fclose(input);

	// Allocate one-dimensional array of strings
	TModule *inst=(TModule*)calloc(MAXY, sizeof(TModule));

	assert((input = fopen(INPUT, "r")));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		char *tempName=strdup(token+(token[0]=='b'?0:1));
		no=modNo(tempName);
		inst[no].name=tempName;
		inst[no].type=token[0];
		token = strtok(NULL, " "); // this gets rid of the ->
		inst[no].connect=calloc(10,sizeof(int));
		if(inst[no].type=='&') { // Conjunction module needs memory
			inst[no].memory=calloc(MAXY,sizeof(int));
		}
		int j=0;
		while( 1 ) {
			if(!(token = strtok(NULL, ", "))) break;
			inst[no].connect[j]=modNo(token);
			j++;
		}

		count++;
	}

	printf("Read in %d lines\n", count);

	fclose(input);
	if (line)
	free(line);

	return inst;
}

int act(int m, TModule *module) {

	TPulse in=pull(m, module);
	TPulse out;

	out.val=0;
	out.from=m;

	switch(module[m].type) {
/* Flip-flop modules (prefix %) are either on or off; they are initially off. If a flip-flop module receives a high pulse, it is ignored and nothing happens. However, if a flip-flop module receives a low pulse, it flips between on and off. If it was off, it turns on and sends a high pulse. If it was on, it turns off and sends a low pulse.*/
	case '%': // Flip-flop
		if(in.val == 2) return 0;
		if(in.val == 1) {
			if(module[m].state) out.val = 1;
			else out.val = 2;
			module[m].state=!module[m].state;
		}
		break;
/* Conjunction modules (prefix &) remember the type of the most recent pulse received from each of their connected input modules; they initially default to remembering a low pulse for each input. When a pulse is received, the conjunction module first updates its memory for that input. Then, if it remembers high pulses for all inputs, it sends a low pulse; otherwise, it sends a high pulse. */
	case '&': // Conjunction
		module[m].memory[in.from] = in.val;
		out.val=1; // if it remembers high pulses for all inputs, it sends a low pulse;
		for(int h=0; h<MAXY; h++) if(module[m].memory[h]==1) out.val=2; // otherwise, it sends a high pulse.
		break;
/* There is a single broadcast module (named broadcaster). When it receives a pulse, it sends the same pulse to all of its destination modules. */
	case 'b':
		out.val=in.val;
	}
	
	for(int q=0; module[m].connect[q]; q++) {
		push(module[m].connect[q], module, out);
	}
	return 0;
}

int main(int argc, char *argv[]) {

	int i=0;
//	array = readInput();
	TModule *module = readInput();

	int start=modNo("broadcaster");

	for(i=1; module[i].name; i++) { // Initialize memory for Conjunction modules
		for(int q=0; module[i].connect[q]; q++) {
			if(module[module[i].connect[q]].type=='&') { // If connected module type is &
				module[module[i].connect[q]].memory[i]=1;
			}
		}
	}

	for(i=1; module[i].name; i++) {
		printf("Module %d (%s), type %c, connects to ", i, module[i].name, module[i].type);
		for(int q=0; module[i].connect[q]; q++) {
			printf("%d (%s), ", module[i].connect[q], module[module[i].connect[q]].name);
		}
		if(module[i].type=='&') {
			printf("\n\tMemory: ");
			for(int h=0; h<MAXY; h++) if(module[i].memory[h]) printf("%d (%s): %d",
				h, module[h].name, module[i].memory[h]);
		}
		if(i == start) printf("[START]");
		printf("\n");
	}

	TPulse button = {1,0};
	for(int rep=0; rep<1000; rep++) {
		push(start, module, button);

		while(1) {
			int something=0;
			for(i=1; module[i].name; i++) {
				if(hasInput(i, module)) {
					printf("There is input waiting for module %d (%s) (queue from %d to %d)\n", i, module[i].name, module[i].idxbottom, module[i].idxtop);
					something=1;
					act(i, module);
				}
			}
			if(!something) break;
		}
	}
//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	printf("Pulses: %ld low * %ld high = %ld\n", pulses[1], pulses[2], pulses[1] * pulses[2]);

	return 0;
}
