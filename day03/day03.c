#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary definitions, set as required

#define INPUT "input.txt"
#define MAXX 140
#define MAXY 140
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Print a two-dimensional array
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(map[y][x]) printf("%c", map[y][x]);
			else printf("_");
		}
		printf("\n");
	}
	printf("\n");
}
// Full block character for maps █

void printPN (int **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(map[y][x]) printf("%d", map[y][x]%10);
			else printf("_");
		}
		printf("\n");
	}
	printf("\n");
}

void printCPM (int **cpm, int **cogno, int **partno, int *partval) {

	int maxcog=0, maxpart=0;
	int y, x;

	for(y=1; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(cogno[y][x]) maxcog=cogno[y][x];
			if(partno[y][x]) maxpart=partno[y][x];
		}
	}

	printf("Found %d cogs and %d parts.\n", maxcog, maxpart);

	for(y=0; y<=maxcog; y++) {
		printf("Cog %d: ", y);
		for(x=0; x<=maxpart; x++) {
			if(cpm[y][x]) printf("%d (%d), ", x, partval[x]);
		}
		printf("\n");
	}
}

// Read input file line by line (e.g., into an array)
char **readInput() {
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
	int x=0, y=0;
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int isnum(char c) {
	return ((c>='0') && (c<='9'));
}

int main(int argc, char *argv[]) {

	int x=0, y=0, dx, dy, pn=1, cn=0, inpart=0;
	char **clean=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) clean[iter]=calloc(MAXX,sizeof(char));
	int change = 0;
	int *partval;

	int **cpm; // Cog-Part Matrix

	int **partno=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) partno[iter]=calloc(MAXX,sizeof(int));

	int **cogno=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) cogno[iter]=calloc(MAXX,sizeof(int));

	char **map=readInput();

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXY; x++) {
			if((map[y][x]!='.') &&
			    ((map[y][x]<'0') || (map[y][x]>'9'))) { // Symbol, all numbers around it count
				for(dy=-1; dy<=1; dy++) {
					for(dx=-1; dx<=1; dx++) {
						if(y+dy<0) break;
						if(x+dx<0) break;
						if(x+dx>=MAXX) break;
						if(y+dy>=MAXY) break;
						if((map[y+dy][x+dx]>='0')&&(map[y+dy][x+dx]<='9')) clean[y+dy][x+dx]=map[y+dy][x+dx];
					}
				}
			}
			if(isnum(map[y][x])) {
				partno[y][x]=pn;
				inpart=1;
			}
			else {
				if(inpart) pn++;
				inpart=0;
			}
			if(map[y][x]=='*') cogno[y][x]=++cn;
		}
	}

	cn++; pn++; // Make sure there's room

	cpm=calloc(cn,sizeof(int*));
	for(int iter=0; iter<cn; iter++) cpm[iter]=calloc(pn,sizeof(int));

	// Array to hold actual values of parts
	partval=calloc(pn,sizeof(int));

	// Fill CPM
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXY; x++) {
			if(map[y][x]!='*') continue;
			for(dy=-1; dy<=1; dy++) {
				for(dx=-1; dx<=1; dx++) {
					if(y+dy<0) break;
					if(x+dx<0) break;
					if(x+dx>=MAXX) break;
					if(y+dy>=MAXY) break;
					if(isnum(map[y+dy][x+dx])) cpm[cogno[y][x]][partno[y+dy][x+dx]]++;
				}
			}
		}
	}

	while(1) {
		change=0;
		for(y=0; y<MAXY; y++) {
			for(x=0; x<MAXY; x++) {
				if(isnum(clean[y][x])) {
					if(x>0) {
						if(isnum(clean[y][x]) && 
						   !isnum(clean[y][x-1]) &&
						   isnum(map[y][x-1])) {
							clean[y][x-1]=map[y][x-1];
							change++;
//							printf("- %d,%d,%c\n",y,x,map[y][x+1]);
						}
					}
					if(x<MAXX-1) {
						if(isnum(clean[y][x]) && 
						   !isnum(clean[y][x+1]) &&
						   isnum(map[y][x+1])) {
							clean[y][x+1]=map[y][x+1];
							change++; 
//							printf("+ %d,%d,%c\n",y,x,map[y][x+1]);
						}
					}

				}
			}
		}	
		if(!change) break;
	}

	// Fill partval
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXY; x++) {
			if(partno[y][x]) {
				partval[partno[y][x]]=atoi(clean[y]+x);
				x+=strlen(clean[y]+x);
			}
		}
	}
	

//	printMap(clean);
//	printPN(partno);
	printCPM (cpm, cogno, partno, partval);

	int sum=0;

	for(y=1; y<cn; y++) {
		int ratcount=0;
		int rat=1;
		for(x=0; x<pn; x++) {
			if(cpm[y][x]) {
				rat=rat*partval[x];
				ratcount++;
			}
		}
		if(ratcount!=2) rat=0;
		sum+=rat;
		printf("Ratio %d: %d (%d), running sum %d\n", y, rat, ratcount, sum);
	}

	return 0;
}
