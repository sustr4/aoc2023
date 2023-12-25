#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>
#include<math.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 300
#define MAXY 300
#define LOW 200000000000000
#define HIGH 400000000000000 /**/
/*#define INPUT "unit1.txt"
#define MAXX 5
#define MAXY 5
#define LOW 7
#define HIGH 27 /**/

// Point structure definition
typedef struct {
	long *p;
	long *v;
} TStone;

// Read input file line by line (e.g., into an array)
TStone *readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	TStone *inst=(TStone*)calloc(MAXX, sizeof(TStone));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into array
		inst[count].p=calloc(3, sizeof(long));
		inst[count].v=calloc(3, sizeof(long));
		sscanf(line,"%ld, %ld, %ld @ %ld, %ld, %ld",
			&(inst[count].p[0]),
			&(inst[count].p[1]),
			&(inst[count].p[2]),
			&(inst[count].v[0]),
			&(inst[count].v[1]),
			&(inst[count].v[2]));

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

void printStone(int i, TStone *stone) {
	printf("%d: ", i);
	for(int q=0; q<3; q++) printf("%ld, ", stone[i].p[q]);
	printf(" (");
	for(int q=0; q<3; q++) printf("%ld, ", stone[i].v[q]);
	printf(")\n");
}

void printSMT2(TStone *stone) {

	for(int i=0; i<4; i++) printf("(declare-const %c Real)\n", 's'+i);
	for(int i=0; i<3; i++) printf("(declare-const %c Real)\n", 'a'+i);
	for(int i=0; i<3; i++) printf("(declare-const %c Real)\n", 'x'+i);
	printf("(declare-const i Real)\n");
	printf("\n");
	// Generate smp2
	for(int i=0; i<4; i++) {
		for(int y=0; y<3; y++) {
			// Preamble
			printf("(assert (= ");
			// Left side:
			printf("(%c %ld (* %c %ld)",
				stone[i].v[y]<0?'-':'+',
				stone[i].p[y],
				's'+i,
				labs(stone[i].v[y]));
			
			printf(") (");
			// Right side:
			printf("+ %c (* %c %c)",
				'x'+y,
				's'+i,
				'a'+y);
			// End of line
			printf(")))\n");
		}
	}
	printf("(assert (= i (+ x (+ y z))))");
	printf("(check-sat)\n");
//	for(int i=0; i<3; i++) printf("(get-value (%c))\n", 'a'+i);
//	for(int i=0; i<3; i++) printf("(get-value (%c))\n", 'x'+i);
	printf("(get-value (i))\n");

}

void assign(double *to, double *from) {
	for(int q=0; q<3; q++) to[q]=from[q];
}

void printHumanReadable(TStone *stone) {

	for(int i=0; i<3; i++) {
		for(int y=0; y<3; y++) {
			// Left side:
			printf("%ld%c%ld%c",
				stone[i].p[y],
				stone[i].v[y]<0?'-':'+',
				labs(stone[i].v[y]),
				's'+i);
			
			printf("=");
			// Right side:
			printf("%ld%c%ldv+%c*%c",
				stone[3].p[y],
				stone[3].v[y]<0?'-':'+',
				labs(stone[3].v[y]),
				'a'+y%3,
				'x'+i);
			// End of line
			if((i!=2) || (y!=2)) printf(",");
			printf("\n");
		}
	}


}

int main(int argc, char *argv[]) {

	TStone *stone = readInput();
	readInput();

	double **after;
	double ***loc;

	after=calloc(MAXX, sizeof(double*));
	loc=calloc(MAXX, sizeof(double**));
	for(int iter = 0; iter < MAXX; iter++) {
		after[iter]=calloc(MAXX, sizeof(double));
		loc[iter]=calloc(MAXX, sizeof(double*));
		for(int iiter = 0; iiter < MAXX; iiter++) loc[iter][iiter]=calloc(3, sizeof(double));
	}

	printSMT2(stone);

	return 0;
}
