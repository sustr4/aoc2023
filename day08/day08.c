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

// Today's special: global variables. Who wants to return two types from a function...
TTurn *node;
char *instructions=0;

int encode(char *node) { // Unique ID for each node.
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

		if(strlen(line)<2) continue; // Skip short (empty} lines

		if(!instructions) { // First line has instructions
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

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return node;
}

int main(int argc, char *argv[]) {

	int i=0, y;	
	node=(TTurn*)calloc(MAXX, sizeof(TTurn));

	node = readInput();

	int starts=0;
	for(i = 0; i<MAXX; i++) if(node[i].name && node[i].name[2]=='A') starts++;

	y=0;
	int *spoint = calloc(starts + 1, sizeof(int));
	for(i = 0; i<MAXX; i++) if(node[i].name && node[i].name[2]=='A') spoint[y++]=i;

	printf("There are %d gosts\n", starts);

	long *cycles = calloc(starts + 1, sizeof(int));
	#pragma omp parallel for
	for(int col=0; col<starts; col++) { // For each ghost (column)
		i = spoint[col];
		y = 0;
		int last = 0;
		int count = 0;
		while( 1 ) { // Calculate path length for ghost (column) col.

	//		if(node[i].left) printf("%s (%d): (%s, %s), (%d %d) -> %c\n", node[i].name, i, node[i].lname, node[i].rname, node[i].left, node[i].right, instructions[y]);

			if(instructions[y]=='L') i=node[i].left;
			else if(instructions[y]=='R') i=node[i].right;
			count ++;

			if(node[i].name[2]=='Z') {
				printf("%d ends up in %s every %d moves\n", col, node[i].name, count-last);
				last=count;
				break;
			}

			y++;
			if((y>=strlen(instructions)) || ((instructions[y]!='L') && (instructions[y]!='R'))) y=0;
		}
		cycles[col]=(long)count;
	}

	long l;
	for(l=cycles[0]; 1; l+=cycles[0]) {
		int fit = 1;

		for(int m = 1; m<starts; m++) {
			if(l % cycles[m]) {
				fit=0;
				break;
			}
		}
		if(fit) break;
	}

	printf("LCD: %ld\n", l);

	return 0;
}
