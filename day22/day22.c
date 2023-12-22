#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
//#define INPUT "unit1.txt"

#define MAXBR 1500

// Brick structure definition
typedef struct {
	int *c;
	int **fills;
	int fell;
} TBrick;

void printBrick(int b, TBrick *brick) {
	for(int j=0; brick[b].fills[j]; j++) printf("[%d,%d,%d], ", brick[b].fills[j][0], brick[b].fills[j][1], brick[b].fills[j][2]);
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
	TBrick *inst=(TBrick*)calloc(MAXBR, sizeof(TBrick));

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
	for(i=0; brick[i].fills; i++) {
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
//	printf("Brick %d falling from ", b);
//	printBrick(b, brick);
	for(int j=0; brick[b].fills[j]; j++) brick[b].fills[j][2]--;
//	printf(" to ");
//	printBrick(b, brick);
//	printf("\n");
	brick[b].fell=1;
}

int main(int argc, char *argv[]) {

	int i=0;	
	TBrick *brick = readInput();

	// Let all fall
	int change;
	while(1) {
		change=0;
		for(i=0; brick[i].fills; i++) {
			if(canfall(i, brick)) {
				fall(i, brick);
				change=1;
			}
		}
		if(!change) break;
	}

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
/*	for(i=0; brick[i].fills; i++) {
		printf("Brick %d", i);
//		for(int j=0; j<6; j++) printf("%3d %c ", brick[i].c[j], j==2?'~':' '); 
		printf("\n\tFills: ");
		printBrick(i, brick);
		if(canfall(i, brick)) printf("\n\tCan fall");
		printf("\n");
	}*/

	int sum = 0;

	for(i=0; brick[i].fills; i++) {

		printf("Trying to remove brick %d\n", i);
		// Make a working copy of the stack but leave out brick i
		TBrick *newstack=(TBrick*)calloc(MAXBR, sizeof(TBrick));
		int new=0;
		for(int y=0; brick[y].fills; y++) {
			if(y==i) {
//				printf("Leaving out brick %d from copy\n", y);
				continue; // Leave out brick i
			}
			int len;
			for(len=0; brick[y].fills[len]; len++);
			newstack[new].fills=calloc(len+1, sizeof(int*));
			for(len=0; brick[y].fills[len]; len++) {
				newstack[new].fills[len]=calloc(3, sizeof(int));
				for(int j=0; j<3; j++) {
					newstack[new].fills[len][j]=brick[y].fills[len][j];
				}
			}
			newstack[new].fell=0;
			new++;
		}
		
		while(1) {
			change=0;
			for(int ii=0; newstack[ii].fills; ii++) {
				if(canfall(ii, newstack)) {
					fall(ii, newstack);
					change=1;
				}
			}
			if(!change) break;
		}

		int locsum=0;
		for(int ii=0; newstack[ii].fills; ii++) if(newstack[ii].fell) locsum++;

		printf("Pulled brick %d, %d bricks fell\n", i, locsum);
		sum+=locsum;
		

		for(int y=0; brick[y].fills; y++) {
			for(int len=0; (newstack[y].fills) && (newstack[y].fills[len]); len++) {
//				printf("Freeing newstack[%d].fills[%d]\n", y, len);
				free(newstack[y].fills[len]);
			}
			free(newstack[y].fills);
		}
		free(newstack);
	}

	printf("%d bricks fell in total\n", sum);

	return 0;
}
