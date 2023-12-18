#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
//#define INPUT "input.txt"
//#define MAXX 141
//#define MAXY 141
#define INPUT "unit1.txt"
#define MAXX 13
#define MAXY 13

#define MAXSTEP 10

int startMax=INT_MAX;

// Point structure definition
typedef struct {
	int x;
	int y;
} TPoint;

// Print a two-dimensional array
void printMap (int **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%d", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █ and border elements ┃━┗┛┏┓
// Color printf("\033[1;31mR \033[1;32mG \033[1;34mB \033[0moff\n");
void printScore (int **map, int ***score) {
	int x,y;

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("\033[1;34m%3d", score[y][x][0]-map[0][0]);
			printf("\033[1;32m%3d", score[y][x][1]-map[0][0]);
			printf("\033[0;31m%d\033[0m ", map[y][x]);
		}
		printf("\n");
	}
}

// Read input file line by line (e.g., into an array)
int **readInput() {
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
	int **map=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(int));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x]-'0';
		y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

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

	printMap(map);

	return map;
}

// 0 right, 1 down, 2 left, 3 up

int newx(int x, int steps, int d) { // New X based on direction
	switch(d) {
		case 0: return x+steps;
		case 2: return x-steps;
		case 1:
		case 3: return x;
	}
	return x;
}
int newy(int y, int steps, int d) { // New X based on direction
	switch(d) {
		case 1: return y+steps;
		case 3: return y-steps;
		case 0:
		case 2: return y;
	}
	return y;
}



int main(int argc, char *argv[]) {

//	TPoint *array;
//	int i=0;	
//	array = readInput();
	int **map=readInput();

        int ***score=calloc(MAXY,sizeof(int**));
        for(int y=0; y<MAXY; y++) {
                score[y]=calloc(MAXX,sizeof(int*));
                for(int x=0; x<MAXX; x++) {
                        score[y][x]=calloc(2,sizeof(int));
                }
        }
        int ***step=calloc(MAXY,sizeof(int**));
        for(int y=0; y<MAXY; y++) {
                step[y]=calloc(MAXX,sizeof(int*));
                for(int x=0; x<MAXX; x++) {
                        step[y][x]=calloc(2,sizeof(int));
                }
        }

	score[0][0][0]=map[0][0];
	score[0][0][1]=map[0][0];
	step[0][0][0]=1;
	step[0][0][1]=1;

	int dx, dy;
	for(int s=1; s<200; s++) {
		// from vertical to horizontal
        	for(int y=0; y<MAXY; y++) {
                	for(int x=0; x<MAXX; x++) {
				if(step[y][x][0]!=s) continue; // Only expand the current step
				int cumul=0;
				for(dx=-1; dx>=0-MAXSTEP; dx--) { // Left
					if(x+dx<0) break;
					cumul+=map[y][x+dx];
					if(!score[y][x+dx][1]) {
						score[y][x+dx][1]=score[y][x][0]+cumul;
						step[y][x+dx][1]=s+1;
					}
					else if(score[y][x+dx][1]>score[y][x][0]+cumul) {
						score[y][x+dx][1]=score[y][x][0]+cumul;
						step[y][x+dx][1]=s+1;
					}
				}
				cumul=0;
				for(dx=1; dx<=MAXSTEP; dx++) { // Right
					if(x+dx>=MAXX) break;
					cumul+=map[y][x+dx];
//					printf("[%d,%d] %d\n", y, x+dx, score[y][x][1]);
					if(!score[y][x+dx][1]) {
						score[y][x+dx][1]=score[y][x][0]+cumul;
						step[y][x+dx][1]=s+1;
					}
					else if(score[y][x+dx][1]>score[y][x][0]+cumul) {
						score[y][x+dx][1]=score[y][x][0]+cumul;
						step[y][x+dx][1]=s+1;
					}
				}
			}
		}
		// from horizontal to vertical
        	for(int y=0; y<MAXY; y++) {
                	for(int x=0; x<MAXX; x++) {
				if(step[y][x][1]!=s) continue; // Only expand the current step
				int cumul=0;
				for(dy=-1; dy>=0-MAXSTEP; dy--) { // Up
					if(y+dy<0) break;
					cumul+=map[y+dy][x];
					if(!score[y+dy][x][0]) {
						score[y+dy][x][0]=score[y][x][1]+cumul;
						step[y+dy][x][0]=s+1;
					}
					else if(score[y+dy][x][0]>score[y][x][1]+cumul) {
						score[y+dy][x][0]=score[y][x][1]+cumul;
						step[y+dy][x][0]=s+1;
					}
				}
				cumul=0;
				for(dy=1; dy<=MAXSTEP; dy++) { // Down
					if(y+dy>=MAXY) break;
					cumul+=map[y+dy][x];
//					printf("[%d,%d] %d\n", y, x+dx, score[y][x][1]);
					if(!score[y+dy][x][0]) {
						score[y+dy][x][0]=score[y][x][1]+cumul;
						step[y+dy][x][0]=s+1;
					}
					else if(score[y+dy][x][0]>score[y][x][1]+cumul) {
						score[y+dy][x][0]=score[y][x][1]+cumul;
						step[y+dy][x][0]=s+1;
					}
				}
			}
		}
	}

	printScore(map,score);

	return 0;
}
