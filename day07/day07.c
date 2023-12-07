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
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

int count = 0;

// Hand structure definition
typedef struct {
	char *cards;
	int bid;
	int type;
} THand;

int handcomp(const void *a, const void *b) // Compare two hands
{
	const THand *da = (const THand *) a;
	const THand *db = (const THand *) b;

	// Sort by rank
	if(da->type > db->type) return 1;
	if(da->type < db->type) return -1;

	// Second rule, sort by individial cards
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
		if(ca=='T') return 1;
		if(cb=='T') return -1;
		if(((ca>='0') && (ca<='9')) && (cb=='J')) return 1;
		if(((cb>='0') && (cb<='9')) && (ca=='J')) return -1;
		if(((cb>='0') && (cb<='9')) && ((ca>='0') && (ca<='9'))) {
			if(ca>cb) return 1;
			if(ca<cb) return -1;
		}
	}
	return 0;
}

// Read input file line by line (e.g., into an array)
THand *readInput() {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	THand *inst=(THand*)calloc(MAXX, sizeof(THand));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<6) continue;

		inst[count].cards=calloc(6,sizeof(char));
		sscanf(line,"%s %d",
			inst[count].cards,
			&(inst[count].bid));

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
		if(((i=='J') && (cnt[i]==5)) || // Probably suboptimal, but at least works
			((i!='J') && (cnt[i] + cnt['J']==5))) { free(cnt); return 7; } // Five
	}
	for(i=(int)'0'; i<=(int)'Z'; i++) {
		if(((i=='J') && (cnt[i]==4)) ||
			((i!='J') && (cnt[i] + cnt['J']==4))) { free(cnt); return 6; } // Four
	}
	for(i=(int)'0'; i<=(int)'Z'; i++) {
		if(((i=='J') && (cnt[i]==3)) ||
			((i!='J') && (cnt[i] + cnt['J']==3))) {
			for(int y=(int)'0'; y<=(int)'Z'; y++) {
				if(y=='J') continue;
				if(y==i) continue;
				if(cnt[y]==2) {free(cnt); return 5; } // Full House
			}
			free(cnt); return 4; // Three
		}
	}
	for(i=(int)'0'; i<=(int)'Z'; i++) {
		if(cnt[i]==2) {
			for(int y=(int)'0'; y<=(int)'Z'; y++) {
				if(i==y) continue; 
				if(cnt[y]==2) {free(cnt); return 3; } // Two Pair
			}
		}
	}
	for(i=(int)'0'; i<=(int)'Z'; i++) {
		if (cnt[i]==2) {free(cnt); return 2; } // Pair
	}
	if(cnt['J']>0) { free(cnt); return 2; }
	else { free(cnt); return 1; }
	

}

int main(int argc, char *argv[]) {

	THand *array;
	int i=0;	
	long win = 0;
	array = readInput();

	#pragma omp parallel for
	for(i=0; i<count; i++) { // Calculate rank for all hands independently
		array[i].type=rank(array[i]);
	}
	
	qsort(array,count,sizeof(THand),handcomp); // Sort hands by rank

	for(i=0; i<count; i++) { // Sum up winnings
		win+=(i+1) * array[i].bid;
		printf("%s - (%d), type %d, rank %d, win %d, total %ld\n", array[i].cards, array[i].bid, array[i].type, i+1, (i+1) * array[i].bid, win);
	}

	return 0;
}
