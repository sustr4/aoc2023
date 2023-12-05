#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>
#include<pthread.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 8
#define MAXY 140
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	long from;
	long to;
	long range;
} TMap;

// Read input file line by line (e.g., into an array)
TMap **readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a two-dimensional arrray of chars
	int x=0,y;
	TMap **inst=calloc(MAXX,sizeof(TMap*));
	for(int iter=0; iter<MAXX; iter++) inst[iter]=calloc(MAXY,sizeof(TMap));

	x=0; y=0;
	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read tokens from single line
		if(!strncmp(line,"seeds:",6)) { // This is the first line with seed numbers
			char *token;
			token = strtok(line+7, " ");
			inst[x][y].to=atol(token);
			inst[x][y++].range=1;
			while( 1 ) {
				if(!(token = strtok(NULL, " "))) break;
				inst[x][y].to=atol(token);
				inst[x][y++].range=1;
			}
		}
		else if((line[0]>='a') && (line[0]<='z')) { // Here starts another set of mappings
			x++; // next column
			y=0; // from top ;-)
		}
		else if((line[0]>='0') && (line[0]<='9')) { // Number, read into array
			assert(x>=0);
			sscanf(line,"%ld %ld %ld",
				&(inst[x][y].to),
				&(inst[x][y].from),
				&(inst[x][y].range));
			count++;
			y++;
		}


	}

	printf("Read %d items into %d columns\n", count, x);

	fclose(input);
	if (line)
	free(line);

	return inst;
}

long findNext(long from, int col, TMap **array) { // Get a number and find a rule that matches.
	int i;

	for(i=0; array[col][i].range; i++) { // Find rule that applies
		if((from>=array[col][i].from) &&
		   (from<array[col][i].from+array[col][i].range)) {
			long offset=from-array[col][i].from;
//			printf("Rule for %ld: %ld -> %ld (%ld), offset %ld\n", from, array[col][i].from, array[col][i].to, array[col][i].range, offset);
			return(array[col][i].to+offset);
		}
	}
	return from; // Not in list, matches itself
}

long findLocation(long from, int col, TMap **array) { // Find next mapping unless already at the end
	if(col>=MAXX) return from; // No more searching, this is it
	return findLocation(findNext(from,col,array), col+1, array);
}

// Structure for thread arguments
struct ThreadArgs {
	int no;
	long start;
	long count;
	TMap **array;
};

long doSeed(struct ThreadArgs *args) { // Thread function to calculate for a single seed range
	long min=LONG_MAX;
	long loc;

	printf("Try form %ld (range %ld)\n", args->start, args->count);
	for(long i=args->start; i<args->start+args->count; i++) {
		loc = findLocation(i,1,args->array);
		if(loc<min) {
			min=loc;
			printf("t%02d: Seed number %ld corresponds to location %ld (min. seen so far %ld).\n", args->no, i, loc, min);
		}
	}
	
	return min;
}


#define NUM_THREADS 20

int main(int argc, char *argv[]) {

	pthread_t threads[NUM_THREADS];
	struct ThreadArgs thread_args[NUM_THREADS];
	void *retval;
	int thrcount=0;

	TMap **array;
	array = readInput();

	long min=LONG_MAX;

	for(int y=0; array[0][y*2].range; y++) { // Every two numbers
		thread_args[y].start = array[0][y*2].to;
		thread_args[y].count = array[0][y*2+1].to;
		thread_args[y].array = array;
		thread_args[y].no = y;

		int rc = pthread_create(&threads[y], NULL, (void *(*)(void *))doSeed, (void *)&thread_args[y]);
		if (rc) {
		    printf("Error: Unable to create thread %d, return code %d\n", y, rc);
		    exit(EXIT_FAILURE);
		}
		thrcount++;
	}


	// Wait for all threads to finish
	for (int y = 0; y < thrcount; y++) {
		int rc = pthread_join(threads[y], &retval);
		if (rc) {
			printf("Error: Unable to join thread %d, return code %d\n", y, rc);
			exit(EXIT_FAILURE);
		}
		printf("Main: Thread %d completed with retval %ld\n", y, (long)retval);
		if((long)retval<min) min=(long)retval;
	}

	printf("The minimum found anywhere is %ld\n", min);

	return 0;
}
