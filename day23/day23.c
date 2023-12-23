#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 141
#define MAXY 141
//#define INPUT "unit1.txt"
//#define MAXX 23
//#define MAXY 23
//#define INPUT "unit2.txt"
//#define MAXX 5
//#define MAXY 5

// Point structure definition
typedef struct {
	int x;
	int y;
	int *nb;
	int *dist;
	int visited;
} TNode;

int max = 0;

// Retrieve nth neighbor from a map
int dy[] = { -1,  0, 0,  1 };
int dx[] = {  0, -1, 1,  0 };
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<4));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
}

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

void printDist (int **dist) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%d", dist[y][x]%10);
		}
		printf("\n");
	}
}

int isnode(char **map, int y, int x) {
	if(map[y][x]=='#') return 0;
	int cnt=0;
	for(int n = 0; n<4; n++) {
		if(mapnb(map, y, x, n)=='.') cnt++;
		else if(mapnb(map, y, x, n)=='v') cnt++;
		else if(mapnb(map, y, x, n)=='>') cnt++;
	}

//	printf("Found %d nbs for [%d,%d]\n", cnt, x, y);

	if(cnt!=2) return 1;
	else return 0;
}

void printNodes (TNode *node, int **noderef, char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(map[y][x]=='#') {
				printf("\033[0;31m#\033[0m");
				continue;
			}
			if(noderef[y][x]>=0) printf("\033[1;33m%d", noderef[y][x]%10);
			else printf(".");
			printf("\033[0m");
		}
		printf("\t");
		for(int nn=0; (node[nn].x) || (node[nn].y); nn++)
			if(node[nn].y==y) {
				printf("Node %d [%d,%d] ", nn, node[nn].x, node[nn].y);
				for(int nb=0; node[nn].dist[nb]; nb++) printf("-> %d (%d), ",node[nn].nb[nb], node[nn].dist[nb]);
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

	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int countnodes(char **map) {
	int ret=0;
	for(int y=0; y<MAXY; y++) {
		for(int x=0; x<MAXX; x++) {
			if(isnode(map, y, x)) ret++;
		}
	}
	return ret;
}

int step(char **map, TNode *node, int nn, int dist) {

	if(node[nn].visited) goto cleanup;
	node[nn].visited=1;

	if(node[nn].y>=MAXY-1) {
		if(dist>max) {
			max=dist;
			printf("New longest path found %d through ", max);
			for(int l=0; (node[l].y) || (node[l].x); l++) if(node[l].visited) printf("%d,", l);
			printf("\n");
			goto cleanup;
		}
	}

	for(int nb=0; node[nn].dist[nb]; nb++) {
		if(node[node[nn].nb[nb]].visited) continue;
		step(map, node, node[nn].nb[nb], dist + node[nn].dist[nb]);
	}
cleanup:
	node[nn].visited=0;
	return 0;
}

int main(int argc, char *argv[]) {

	TNode *node = NULL;
	char **map=readInput();

	int x=0, y=0;

	int nodesNo = countnodes(map);
	printf("Map has %d nodes.\n", nodesNo);

	node=(TNode*)calloc(nodesNo+1, sizeof(TNode));
	int **noderef=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) noderef[iter]=calloc(MAXX,sizeof(int));
	for(y=0; y<MAXY; y++)
		for(x=0; x<MAXX; x++) noderef[y][x]=-1;

	int nn = 0;
	for(y=0; y<MAXY; y++)
		for(x=0; x<MAXX; x++)
			if(isnode(map, y, x)) {
//				printf("Adding node %d at [%d,%d]\n", nn, x, y);
				node[nn].x=x;
				node[nn].y=y;
				node[nn].nb=calloc(5,sizeof(int));
				node[nn].dist=calloc(5,sizeof(int));
				noderef[y][x]=nn;
				nn++;
			}

	for(nn=0; nn<nodesNo; nn++) {

//		printf("Finding neighbors of node %d\n", nn);
		int newnb=0;
		for(int n=0; n<4; n++) {
			if(node[nn].y+dy[n]<0) continue;
			if(node[nn].x+dx[n]<0) continue;
			if(node[nn].y+dy[n]>MAXY-1) continue;
			if(node[nn].x+dx[n]>MAXX-1) continue;
			if(map[node[nn].y+dy[n]][node[nn].x+dx[n]]=='#') continue;
	
//			printf("Trying direction %d\n", n);
	
			int **dist=calloc(MAXY,sizeof(int*));
			for(int iter=0; iter<MAXY; iter++) dist[iter]=calloc(MAXX,sizeof(int));

			dist[node[nn].y][node[nn].x]=1;
			dist[node[nn].y+dy[n]][node[nn].x+dx[n]]=2;
			int s = 2;
			while(1) {
				int change=0;
				for(y=0; y<MAXY; y++) {
					for(x=0; x<MAXX; x++) {
						if(dist[y][x]!=s) continue;
						if(noderef[y][x]>=0 && (noderef[y][x]!=nn)) {
//							printf("Node %d found at distance %d\n", noderef[y][x], dist[y][x]-1);
							node[nn].nb[newnb]=noderef[y][x];
							node[nn].dist[newnb]=dist[y][x]-1;
							newnb++;
							continue;
						}
						for(int n = 0; n<4; n++) {
							char nb = mapnb(map, y, x, n);
							if(!nb) continue;
							switch(map[y+dy[n]][x+dx[n]]) {
							case '#':
								continue;
								break; // Yea, I know
							case '>':
							case 'v':
							case '.':
								if((dist[y+dy[n]][x+dx[n]]==0)) dist[y+dy[n]][x+dx[n]] = s+1;
								change = 1;
								break;
							}
						}
					}
				}
				
				if(!change) break;
				s++;
			}

//			printDist(dist);

			for(int iter=0; iter<MAXY; iter++) free(dist[iter]);
			free(dist);
		}
	}


	printNodes(node, noderef, map);

	step(map, node, 0, 0);

	return 0;
}
