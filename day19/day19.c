#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 20
#define MAXY 600
//#define INPUT "unit1.txt"

// Point structure definition
typedef struct {
	char cat;
	char op;
	int val;
	int send;
} TRule;

typedef struct {
	char *name;
	TRule *rule;
} TQueue;

TQueue *queue=NULL;
char **qs=NULL;
int **part=NULL;

int catno(char c) {
	switch(c) {
	case 'x': return 0;
	case 'm': return 1;
	case 'a': return 2;
	case 's': return 3;
	}
	assert(0);
	return -1;
}

int qn(char *name) { // Get number from queue name
	if(!qs) qs=calloc(MAXY,sizeof(char*));

	int i;

	for(i=0; qs[i]; i++) if(!strcmp(name,qs[i])) return i;

//	printf("Inserting %s into the queue list as No. %d.\n", name, i);
	qs[i]=strdup(name);
	return i;
}

// Comparator function example
int comp(const void *a, const void *b)
{
	const int *da = (const int *) a;
	const int *db = (const int *) b;
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
// Full block character for maps █ and border elements ┃━┗┛┏┓
// Color printf("\033[1;31mR \033[1;32mG \033[1;34mB \033[0moff\n");

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
TRule *readInput() {
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
	// char **queue=(char**)calloc(MAXX, sizeof(char*));

	// Allocate a two-dimensional arrray of chars
	int x, y=0;
	queue=calloc(MAXY, sizeof(TQueue));
	part=calloc(MAXY, sizeof(int*));

	int parts=0;

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) {
			x=0;
			y=0;
			parts=1; // Rules ended, now reading parts
			continue;
		}

		if(!parts) {
			// Read tokens from single line
			char *token;
			token = strtok(line, "{");
			y=qn(token);
			queue[y].name = strdup(token);
			queue[y].rule=(TRule*)calloc(MAXX, sizeof(TRule));
			x=0;
			while( 1 ) {
				if(!(token = strtok(NULL, ","))) break;
//				printf("Next rule: %s (%ld)\n", token, strlen(token));

				if(token[strlen(token)-1]=='}') {
					token[strlen(token)-1]=0;
					queue[y].rule[x].op='G';
					queue[y].rule[x].send=qn(token);
					break;
				}
				queue[y].rule[x].cat=token[0];
				queue[y].rule[x].op=token[1];
				queue[y].rule[x].val=atoi(token+2);
				int f;
				for(f=0; token[f]!=':'; f++);
				queue[y].rule[x].send=qn(token+f+1);
				x++;
			}

//			printf("\nNEXT LINE\n");
		}
		{
			part[y]=calloc(4, sizeof(char));
			sscanf(line,"{x=%d,m=%d,a=%d,s=%d}",
				&(part[y][catno('x')]),
				&(part[y][catno('m')]),
				&(part[y][catno('a')]),
				&(part[y][catno('s')]));
			y++;
			part[y]=NULL;
		}

		count++;
	}

	printf("Read %d parts\n",y);

	fclose(input);
	if (line)
	free(line);

	return 0;
}

int next(int p, int q) {
	int r;
	for(r=0; queue[q].rule && queue[q].rule[r].op; r++) {
		switch (queue[q].rule[r].op) {
		case '>':
			if(part[p][catno(queue[q].rule[r].cat)] > queue[q].rule[r].val) return queue[q].rule[r].send;
			break;
		case '<':
			if(part[p][catno(queue[q].rule[r].cat)] < queue[q].rule[r].val) return queue[q].rule[r].send;
			break;
		case 'G':
			return queue[q].rule[r].send;
			break;
		}

	}
	return q;
}

int main(int argc, char *argv[]) {

//	TRule *array;
	int i=0;	
//	array = readInput();
	readInput();

	int **ms=(int**)calloc(4, sizeof(int*));
	int msidx[] = {0,0,0,0};
	for(int j=0; j<4; j++) ms[j]=calloc(MAXY, sizeof(int));

	int start=qn("in");
	int accept=qn("A");
	int refuse=qn("R");

	printf("Start Queue has No. %d, Accept queue has No %d and refuse queue has No %d\n", start, accept, refuse);

	for(i=0; i<MAXY; i++) {
		if(!queue[i].rule) continue;
		printf("Queue %d (%s):\n", i, qs[i]);
		for(int r=0; queue[i].rule && queue[i].rule[r].op; r++) {
			printf("\t%2d: %c %c %d -> %d (%s)\n", r, queue[i].rule[r].cat, queue[i].rule[r].op, queue[i].rule[r].val, queue[i].rule[r].send, qs[queue[i].rule[r].send]);
			if(queue[i].rule[r].op=='<') {
				int c = catno(queue[i].rule[r].cat);
				ms[c][msidx[c]++] = queue[i].rule[r].val-1;
				continue;
			}
			if(queue[i].rule[r].op=='>') {
				int c = catno(queue[i].rule[r].cat);
				ms[c][msidx[c]++] = queue[i].rule[r].val;
				continue;
			}
		}
	}

	int sum=0;

	for(i=0; part[i]; i++) {
//	for(i=0; i<1; i++) {
		printf("%3d: %d, %d, %d, %d\n", i,
			part[i][catno('x')],
			part[i][catno('m')],
			part[i][catno('a')],
			part[i][catno('s')]);

		int q=start;
		while(1) {
			printf("%d (%s) -> ", q, qs[q]);
			q=next(i, q);
			printf("%d (%s).\n", q, qs[q]);
			if(q==accept) {
				sum+=part[i][catno('x')] + part[i][catno('m')] + part[i][catno('a')] + part[i][catno('s')];
				break;
			}
			if(q==refuse) break;
		}
	}

	printf("Sum: %d\n", sum);

	int max=0;
	for(int j=0; j<4; j++) { // Cap each list, sort, and count
		ms[j][msidx[j]++]=4000;
//		printf("%9d",msidx[j]);
		qsort(ms[j],++msidx[j],sizeof(int),comp); // The ++ includes one of the trailing zeroes into the sort
		if(max<msidx[j]) max=msidx[j];
	}

	for(int k=0; k<max; k++) {
		for(int j=0; j<4; j++) {
			printf("%9d",ms[j][k]);
		}
		printf("\n");
	}

	long long sum2=0;
	long long checksum2=0;

	int l[4];
//	#pragma omp parallel for shared(sum2,checksum2)
	for(l[0]=1; l[0]<msidx[0]; l[0]++) {
		printf("%d\n", l[0]);
		for(l[1]=1; l[1]<msidx[1]; l[1]++) {
			for(l[2]=1; l[2]<msidx[2]; l[2]++) {
				for(l[3]=1; l[3]<msidx[3]; l[3]++) {
					for(int j=0; j<4; j++) {
						part[0][j]=ms[j][l[j]];
//						printf("%9d",part[0][j]);
					}
//					printf("\n");
					int q=start;
					long long fac=1;
					for(int j=0; j<4; j++) fac*=((long long)ms[j][l[j]]-(long long)ms[j][l[j]-1]);
					checksum2+=fac;
					while(1) {
//						printf("%d (%s) -> ", q, qs[q]);
						q=next(0, q);
//						printf("%d (%s).\n", q, qs[q]);
						if(q==accept) {
							sum2+=fac;
							break;
						}
						if(q==refuse) break;
					}
				}
			}
		}
	}
	printf("     Sum 2: %15lld\n", sum2);
	printf("  Checksum: %15lld\n", checksum2);
	printf("Totalcheck: %15lld\n", (long long)4000*(long long)4000*(long long)4000*(long long)4000);
}
