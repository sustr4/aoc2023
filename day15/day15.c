#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 4000
#define MAXY 100
//#define INPUT "unit1.txt"

// Point structure definition
typedef struct {
	char *label;
	char op;
	int val;
} TStep;

typedef struct {
	char *label;
	int val;
} TLens;

// Read input file line by line (e.g., into an array)
char **readInput() {
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

	char **map=calloc(MAXX,sizeof(char*));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read tokens from single line
		char *token;
		token = strtok(line, ",");
		map[count++]=strdup(token);
		while( 1 ) {
			if(!(token = strtok(NULL, ","))) break;
			map[count++]=strdup(token);
		}
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int hash(char *str) { // Calculate hash
	int sum=0;
	for(int y=0; y<strlen(str); y++) {
		sum+=(int)(str[y]);
		sum*=17;
		sum=sum % 256;
	}
	return sum;
}

TStep fromString(char *str) { // Read complete instruction from string
	TStep s;
	int i;

	s.label=strdup(str);
	for(i=0; i<strlen(str); i++) {
		if((str[i]=='-') || (str[i]=='=')) {
			s.op=s.label[i];
			s.label[i]=0;
			break;
		}
	}
	s.val = atoi(str+i+1);

	return s;
}

TLens push(TLens *box, TLens lens) { // Push lens into the box
	int i;
	TLens ret = {NULL, 0};

	for(i=0; box[i].label; i++) {
		if(!strcmp(box[i].label,lens.label)) { // Exchange if lens present
			ret.label=strdup(lens.label);
			ret.val=box[i].val;
			box[i].val=lens.val;
			return ret;
		}
	}
	box[i].label=strdup(lens.label); // Add to end
	box[i].val=lens.val;

	return ret;
}

TLens pull(TLens *box, char *label) { // Pull lens out of the box

	int i,y;
	TLens ret = {NULL, 0};

	for(i=0; box[i].label; i++) {
		if(!strcmp(box[i].label,label)) { // Pull if found
			ret.label=label;
			ret.val=box[i].val;
			for(y = i; box[y+1].label; y++) box[y]=box[y+1];
			box[y].label=NULL;
			box[y].val=0;
		}
	}

	return ret;
}

int printBox(TLens *box) { // Print but also calculate focusing power
	int fl=0;
	for(int i=0; box[i].label; i++) {
		printf("[%s %d] ", box[i].label, box[i].val);
		fl+=(i+1)*box[i].val;
	}
	printf("\tfl=%d\n", fl);

	return fl;
}

int main(int argc, char *argv[]) {

	TStep *step = calloc(MAXX, sizeof(TStep));

	TLens **box = calloc(256, sizeof(TLens *));
	for(int t=0; t<256; t++) box[t]=calloc(MAXY, sizeof(TLens));

	int i=0, sumsum=0;	
	char **map=readInput();

	for(i=0; map[i]; i++) sumsum+=hash(map[i]);
	printf("Sum of hashes: %d\n", sumsum);

	for(i=0; map[i]; i++) {
		step[i]=fromString(map[i]);

		printf("\nAfter %s\t%s (%d) %c %d\n", map[i], step[i].label, hash(step[i].label), step[i].op, step[i].val);

		if(step[i].op=='=') {
			TLens hand = { step[i].label, step[i].val };
			push(box[hash(hand.label)], hand);
		}
		else if(step[i].op=='-') {
			pull(box[hash(step[i].label)], step[i].label );
		}
		

		int total=0;
		for(int y=0; y<256; y++) {
			if(box[y][0].label) {
				printf("Box %3d: ", y);
				total+=(y+1) * printBox(box[y]);
			}
		}
		printf("Current focus: %d\n", total);
	}


	return 0;
}
