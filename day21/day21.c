#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 131
#define MAXY 131
int GOAL=196;
//#define INPUT "unit1.txt"
//#define MAXX 11
//#define MAXY 11
//#define GOAL 6

// Print a two-dimensional array
void printDist (int **dist) {
	int x,y;
	for(y=0; y<MAXY*7; y++) {
		for(x=0; x<MAXX*7; x++) {
			if(dist[y][x])	printf("%d", dist[y][x]%10);
			else printf(" ");
		}
		printf("\n");
	}
}

void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY*7; y++) {
		for(x=0; x<MAXX*7; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}

// Retrieve nth neighbor from a map
int dy[] = { -1,  0, 0, 1 };
int dx[] = {  0, -1, 1, 0 };
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<4));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY*7) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX*7)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
}

// Read input file line by line (e.g., into an array)
char **readInput() {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
	char **map=calloc(MAXY*7,sizeof(char*));
	for(int iter=0; iter<MAXY*7; iter++) map[iter]=calloc(MAXX*7,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		for(x=0; x<MAXX; x++) {
			for(int a=0; a<7; a++) {
				for(int b=0; b<7; b++) {
					map[y+a*MAXY][x+b*MAXY] = line[x];
				}
			}
		}
		y++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

long long cnt(int qy, int qx, int **dist) {
	long long ret=0;

	for(int y=MAXY*qy; y<MAXY*(qy+1); y++) {
		for(int x=MAXX*qx; x<MAXX*(qx+1); x++) {
			if((dist[y][x]) && (dist[y][x]<=GOAL+1)) ret ++;
		}
	}
	return ret;
}

int main(int argc, char *argv[]) {

	char **map=readInput();
	int **dist=calloc(MAXY*7,sizeof(int*));
	for(int iter=0; iter<MAXY*7; iter++) dist[iter]=calloc(MAXX*7,sizeof(int));

	int starts=0;
	for(int y=0; y<MAXY*7; y++)
		for(int x=0; x<MAXX*7; x++)
			if(map[y][x]=='S') {
				map[y][x]='.';
				if(++starts==25) dist[y][x]=1;
			}

	GOAL= 26501365 % MAXX + MAXX *3;
	printf("26501365 is like %d. Go %d\n", 26501365 % MAXX, GOAL);

	int s;
	for(s=1; s<=GOAL; s++) {
		for(int y=0; y<MAXY*7; y++) {
			for(int x=0; x<MAXX*7; x++) {
				if(dist[y][x]!=s) continue;
//				printf("[%d][%d]: %d!=%d\n", y, x, dist[y][x], s);
				for(int n=0; n<4; n++) {
					char nb = mapnb(map, y, x, n);
//					printf("\t nb %d: %c\n", n, nb);
					if((nb =='.') &&
						(dist[y+dy[n]][x+dx[n]]==0)) dist[y+dy[n]][x+dx[n]]=s+1;
				}
				
			}
		}
		for(int y=0; y<MAXY*7; y++) 
			for(int x=0; x<MAXX*7; x++) if(dist[y][x]<=s) dist[y][x]=0;
	}

	long long count[7][7];

	int checksum=0;
	for(int y=0; y<7; y++) {
		int linesum=0;
		for(int x=0; x<7; x++) {
			count[y][x]=cnt(y, x, dist);
			printf("%4lld ", count[y][x]);
			linesum+=count[y][x];
			checksum+=count[y][x];
		}
		printf("\t%d\n", linesum);
	}
	

	printf("Reached %d testing tiles.\n", checksum);

	int extend = 26501365 / MAXX ;

	printf("Now .. we need %d tiles up, down, left right...\n", extend);

	long long dirsum = 0;

	dirsum+=count[0][3]+extend*count[0][2]+extend*count[0][4];
	dirsum+=(extend-1)*count[1][2]+(extend-1)*count[1][4];
	dirsum+=count[3][0]+count[3][6];
	dirsum+=count[6][3]+extend*count[6][2]+extend*count[6][4];
	dirsum+=(extend-1)*count[5][2]+(extend-1)*count[5][4];

	int w=2*extend-1;
	dirsum+=w * count[3][3]; // pyramid bottom

	for(w = w-2; w>0; w-=2) dirsum+=w*count[3][3] + w*count[2][3];

	printf("Reached %lld tiles.\n", dirsum);

	return 0;
}
