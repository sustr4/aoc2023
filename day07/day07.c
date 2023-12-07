#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 1001
#define MAXY 26
//#define INPUT "unit2.txt"
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	char *cards;
	char *sorted;
	int bid;
	int type;
} THand;

// Comparator function example
int handcomp(const void *a, const void *b)
{
	const THand *da = (const THand *) a;
	const THand *db = (const THand *) b;

	if(da->type > db->type) return 1;
	if(da->type < db->type) return -1;

	for(int i=0; i<strlen(da->cards); i++) {
		char ca = da->cards[i];
		char cb = db->cards[i];
		if(ca==cb) continue;
		if(ca=='A') return 1;
		if(cb=='A') return -1;
		if(ca=='K') return 1;
		if(cb=='K') return -1;
		if(ca=='Q') return 1;
		if(cb=='Q') return -1;
		if(ca=='J') return 1;
		if(cb=='J') return -1;
		if(ca=='T') return 1;
		if(cb=='T') return -1;
		if(ca>cb) return 1;
		if(ca<cb) return -1;
	}
	return 0;
}

int comp(const void *a, const void *b)
{
	const char *da = (const char *) a;
	const char *db = (const char *) b;
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
// Full block character for maps █

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
THand *readInput() {
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

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	THand *inst=(THand*)calloc(MAXX, sizeof(THand));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	// char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<6) continue;

		// Read into array
		inst[count].cards=calloc(6,sizeof(char));
		sscanf(line,"%s %d",
			inst[count].cards,
			&(inst[count].bid));

		asprintf(&(inst[count].sorted),"%s",inst[count].cards);
		qsort(inst[count].sorted,strlen(inst[count].sorted),sizeof(char),comp);

		// Read tokens from single line
		//char *token;
		//token = strtok(line, ",");
		//while( 1 ) {
		//	if(!(token = strtok(NULL, ","))) break;
		//}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

int rank(THand hand) {
	char *cnt = calloc(255,sizeof(char));
	int i;

	for(i=0; i<strlen(hand.cards);i++) cnt[(int)hand.cards[i]]++; // Count how many times cards appear

//    Five of a kind, 7
//    Four of a kind, 6
//    Full house 5
//    Three of a kind 4
//    Two pair 3
//    One pair 2
//    High card 1
	for(i=(int)'0'; i<=(int)'Z'; i++) {
		if(cnt[i]==5) { free(cnt); return 7; } // Five
		if(cnt[i]==4) { free(cnt); return 6; } // Four
		if(cnt[i]==3) {
			for(int y=(int)'0'; y<=(int)'Z'; y++) {
				if(y==i) continue;
				if(cnt[y]==2) {free(cnt); return 5; } // Full House
			}
			free(cnt); return 4; // Three
		}
		if(cnt[i]==2) {
			for(int y=(int)'0'; y<=(int)'Z'; y++) {
				if(y==i) continue;
				if(cnt[y]==3) {free(cnt); return 5; } // Full House
				if(cnt[y]==2) {free(cnt); return 3; } // Two pair
			}
			free(cnt); return 2;
		}
	}

	free(cnt); return 1;
	

}

int main(int argc, char *argv[]) {

	THand *array;
	int i=0;	
	array = readInput();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; array[i].cards; i++) {
		array[i].type=rank(array[i]);
	}
	
	qsort(array,i,sizeof(THand),handcomp);

	long win = 0;
	for(i=0; array[i].cards; i++) {
		win+=(i+1) * array[i].bid;
		printf("%s - %s - (%d), type %d, rank %d, win %d, total %ld\n", array[i].cards, array[i].sorted, array[i].bid, array[i].type, i+1, (i+1) * array[i].bid, win);
	}

	return 0;
}
