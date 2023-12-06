#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>
#include<math.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"

// Race structure definition
typedef struct {
	double time;
	double dist;
} TRace;

TRace readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;
	TRace retval;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }


	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		char *condense=calloc(strlen(line),sizeof(char));
		int j=0;
		for(int i=0; i<strlen(line); i++)
			if((line[i]>='0') && (line[i]<='9')) condense[j++]=line[i];
		if(count==0) retval.time=atof(condense);
		else if(count==1) retval.dist=atof(condense);
		free(condense);

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return retval;
}

int main(int argc, char *argv[]) {

	TRace r = readInput();

	long t1 = round(0.5*(r.time-sqrt(r.time*r.time-4*r.dist)));
	long t2 = round(0.5*(r.time+sqrt(r.time*r.time-4*r.dist)));

	printf("t1 = %9ld\nt2 = %9ld\n t = %9ld\n",t1, t2, t2 - t1 - 1);

	return 0;
}
