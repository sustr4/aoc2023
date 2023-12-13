#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

long total=0;

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 35
#define MAXY 1001
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	char *map;
	int *ops;
	int maxop;
	int has;
	int needs;
	int ques;
	int *maxoffset;
} TLine;

void multiply(int *vars) {
	int l;
	for(l=0; vars[l]; l++);

	for(int i=0; i<l; i++) {
		for(int y=1; y<5; y++) {
			vars[y*l+i]=vars[i];
		}
	}
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

	// Allocate one-dimensional array of lines
	TLine *inst=(TLine*)calloc(MAXY, sizeof(TLine));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) continue;

		inst[count].ops=calloc(MAXX, sizeof(int));
		inst[count].maxoffset=calloc(MAXX, sizeof(int));

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		asprintf(&(inst[count].map), "%s?%s?%s?%s?%s", token, token, token, token, token);
		for(int ii=0; ii<=strlen(inst[count].map); ii++) {
			if (inst[count].map[ii]=='#') inst[count].has++;
			if (inst[count].map[ii]=='?') inst[count].ques++;
		}
		int y=0;
		while( 1 ) {
			if(!(token = strtok(NULL, ","))) break;
			inst[count].ops[y] = atoi(token);
			inst[count].needs+=inst[count].ops[y];
			y++;
		}
		multiply(inst[count].ops);
		inst[count].needs*=5;
		for(inst[count].maxop=0; inst[count].ops[inst[count].maxop]; inst[count].maxop++);

		inst[count].maxoffset[inst[count].maxop]=strlen(inst[count].map);
		for(int j=inst[count].maxop-1; j>=0; j--) {
			inst[count].maxoffset[j]=inst[count].maxoffset[j+1]-inst[count].ops[j];
		}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

long nextWrap(char* wholemap, int offset, int op, int maxop, int *ops, int *maxoff, long **accel);


long next(char* wholemap, int offset, int op, int maxop, int *ops, int *maxoff, long **accel) {

	char *map=wholemap + offset;

//	printf("%90s (%2ld) (%d/%d: %d)\n", map, strlen(map), op, maxop, ops[op]);

	if(map[0]==0) {
		if(op==maxop) { total++; }
		// TODO: This will need some cheking
		return 1;
	}

	if(op==maxop) { // operational vents used up. Only empties must remain
		int k=0;
		while(map[k]) if(map[k++]=='#') return 0;
	} 

	if(offset>maxoff[op]) {
//		printf("Offset %d, maximum %d\n", offset, maxoff[op]);
		return 0;
	}


	if(map[0]=='.') nextWrap(wholemap, offset + 1, op, maxop, ops, maxoff, accel);

	if(map[0]=='#') {
//		printf("Yes, the hash is reconized\n");
		if(strlen(map)<ops[op]) return 0; // map too short
//		printf("and not too short\n");
		int y;
		for(y=0; y<ops[op]; y++) {
			if(map[y]=='.') return 0; // there is a gap in the run
		}
//		printf("Run for %d, next character is %c.\n",y,map[y]);
		if(map[y]=='.')		nextWrap(wholemap, offset+y+1, op+1, maxop, ops, maxoff, accel); // Skip safely the border character
		else if(map[y]=='?')	nextWrap(wholemap, offset+y+1, op+1, maxop, ops, maxoff, accel); // Skip safely the border character
		else if(map[y]==0)	nextWrap(wholemap, offset+y, op+1, maxop, ops, maxoff, accel); // This will be the final check

		return 0; // The run is too long. A dot or end of string must follow
	}
	
	if(map[0]=='?') {
		map[0]='#';
		nextWrap(wholemap, offset, op, maxop, ops, maxoff, accel); // Like '#'
		map[0]='?';

		nextWrap(wholemap, offset+1, op, maxop, ops, maxoff, accel); // Like '.'
	}

	return 0;
} 

long nextWrap(char* wholemap, int offset, int op, int maxop, int *ops, int *maxoff, long **accel) {

	if(accel[offset][op]>=0) { // Already seen this
		total+=accel[offset][op];
	}
	else { // Fresh combination: let's calculate it
		long ot=total;
		next(wholemap, offset, op, maxop, ops, maxoff, accel);
		accel[offset][op]=total-ot;
	}

	return 0;
}

void debugOut(TLine *array) {

	for(int i=0; array[i].map; i++) {
		printf("l%03d:\n\t%s (%ld)\n\t", i, array[i].map, strlen(array[i].map));
		for(int y=0; array[i].ops[y]; y++) printf("%d, ", array[i].ops[y]);
		for(int y=0; array[i].ops[y]; y++) printf("%d, ", array[i].maxoffset[y]);
		printf("is the maximum offset, respectively\n");
		printf("\t.. has %d hashes, needs %d, therefore misses %d in %d places.\n", array[i].has, array[i].needs, array[i].needs - array[i].has, array[i].ques);
	}

}

int main(int argc, char *argv[]) {

	TLine *array = readInput();

//	debugOut(array);

	for(int line=0; line<MAXY; line++) {
		if(!array[line].map) continue;

		// Initialize new acceleration table
		long **accel=(long**)calloc(strlen(array[line].map)+1, sizeof(long*));
		for(int iter=0; iter<=strlen(array[line].map); iter++) {
			accel[iter]=(long*)malloc((array[line].maxop+1) * sizeof(long));
			for(int c=0; c<array[line].maxop+1; c++) accel[iter][c]=-1;
		}

		next(array[line].map, 0, 0, array[line].maxop, array[line].ops, array[line].maxoffset, accel);
		
		for(int iter=0; iter<=strlen(array[line].map); iter++) free(accel[iter]);
		free(accel);
	}

	printf("Total: %ld\n", total);

}
