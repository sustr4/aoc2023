#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 1496
#define MAXY 1496
#define CONNS 3345
//#define INPUT "unit1.txt"
//#define MAXX 15
//#define MAXY 15
//#define CONNS 33

char **modname;
int **connTo;
int **connection;
int **quickconnect;

int modno(char *name) {

	int i;
	for(i=0; modname[i]; i++) {
		if(!strcmp(modname[i], name)) return i;
	}
	modname[i]=strdup(name);
	return i;
}

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

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

void printGrp (int **map, int *group) {
	int x,y;
	printf("    ");
	for(y=0; y<MAXY; y++) {
		printf("%2d", y);
	}
	printf("\n");
	for(y=0; y<MAXY; y++) {
		printf("%2d: ",y);
		for(x=0; x<MAXX; x++) {
			if(map[y][x]) printf("\033[1;3%dm", group[y]);
			else printf("\033[0m");
			printf("%2d", map[y][x]);
		}
		printf("\033[0m\n");
	}
}

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
int readInput() {
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
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	connTo=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) connTo[iter]=calloc(MAXX,sizeof(int));

	modname=calloc(MAXY, sizeof(char**));

	int conns=0;

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read tokens from single line
		char *token;
		token = strtok(line, ": ");
		int from = modno(token);
		while( 1 ) {
			if(!(token = strtok(NULL, " "))) break;
			int to = modno(token);
			connTo[from][to] = 1;
			connTo[to][from] = 1;
			conns++;
		}

		count++;
	}

	int cn=0;
	connection=calloc(conns+1, sizeof(int*));
	for(int x=0; x<MAXY-1; x++) {
		for(int y=x+1; y<MAXY; y++) {
			if(!connTo[x][y]) continue;
			connection[cn]=calloc(2,sizeof(int));
			connection[cn][0]=x;
			connection[cn][1]=y;
			cn++;
		}
	}

	fclose(input);
	if (line)
	free(line);

//	printMap(map);

	return 0;
//	return inst;
//	return map;
}

int colnb(int *group, int **ct, int from, int g) {
	group[from]=g;
	for(int x=1; x<=quickconnect[from][0]; x++) {
		int nx = quickconnect[from][x];
		if((ct[from][nx]) && (!group[nx])) colnb(group, ct, nx, g);
	}
	return 0;
}

int findGrp(int *group, int **ct) {
	int y;
	int grp=1;

	// Find first module with no group

	while(1) {
		for(y=0; (y<MAXY) && (group[y]!=0); y++);
		if(y==MAXY) break;

		colnb(group, ct, y, grp);

		grp++;
	}

	return grp -1;
}

void fillqc() {

	quickconnect = calloc(MAXY+1, sizeof(int*));

	for(int y=0; y<MAXY; y++) {
		int conns=0;
		for(int x=0; x<MAXY; x++) if(connTo[y][x]) conns++;
		quickconnect[y]=calloc(conns+1, sizeof(int));
		quickconnect[y][0]=conns;
		conns=1;
		for(int x=0; x<MAXY; x++) if(connTo[y][x]) quickconnect[y][conns++]=x;
	}
}

int main(int argc, char *argv[]) {

//	TPoint *array;
//	int i=0;	
//	array = readInput();
	int rgFrom=0;
	int rgTo=0;
	readInput();

	fillqc();

	printf("argc == %d\n", argc);

	if(argc>=3) {
		rgFrom=atoi(argv[1]);
		rgTo=atoi(argv[2]);
	}

	int r0, r1, r2;
/*	int t[3];


	printf("In this example, if you disconnect the wire between %d/%d, the wire between %d/%d, and the wire between %d/%d, you will divide the components into two separate, disconnected groups.\n",
		modno("pzl"),
		modno("hfx"),
		modno("cmg"),
		modno("bvb"),
		modno("jqt"),
		modno("nvd"));
	for(t[0]=0; (connection[t[0]][0]!=modno("pzl")) && (connection[t[0]][1]!=modno("hfx")); t[0]++);
	printf("Test connection %d\n", t[0]);
	for(t[1]=0; (connection[t[1]][0]!=modno("cmg")) && (connection[t[1]][1]!=modno("bvb")); t[1]++);
	printf("Test connection %d\n", t[1]);
	for(t[2]=0; (connection[t[2]][0]!=modno("jqt")) && (connection[t[2]][1]!=modno("nvd")); t[2]++);
	printf("Test connection %d\n", t[2]);*/

	int **tempConn=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) {
		tempConn[iter]=calloc(MAXX,sizeof(int)); }
	for(int iter=0; iter<MAXY; iter++) {
		memcpy(tempConn[iter], connTo[iter], MAXY*sizeof(int));
	}

	int conns;
	for(conns=0; connection[conns]; conns++);
	printf("There are %d connections in total\n", conns);
//	#pragma omp parallel for shared(found)
	if(!rgTo) rgTo=conns-3;
	printf("Solving between %d and %d.\n", rgFrom, rgTo);
	for(r0=rgFrom; r0<=rgTo; r0++) {
		printf("%d\n", r0);
		for(r1=r0+1; r1<conns-1; r1++) {
			for(r2=r1+1; r2<conns; r2++) {

//				for(int iter=0; iter<MAXY; iter++) {
//					memcpy(tempConn[iter], connTo[iter], MAXY*sizeof(int));
//				}
//				printf("Removing %d, %d and %d\n", r[0], r[1], r[2]);

				tempConn[connection[r0][0]][connection[r0][1]] = 0;
				tempConn[connection[r0][1]][connection[r0][0]] = 0;
				tempConn[connection[r1][0]][connection[r1][1]] = 0;
				tempConn[connection[r1][1]][connection[r1][0]] = 0;
				tempConn[connection[r2][0]][connection[r2][1]] = 0;
				tempConn[connection[r2][1]][connection[r2][0]] = 0;
				
				int *group=calloc(MAXY, sizeof(int));
				int grcnt = findGrp(group, tempConn);

				if(grcnt>1) {
//				if((grcnt>1) || ((r[0]==0)&&(r[1]==1)&&(r[2]==2))) {
//					printGrp(tempConn, group);

					int *counts=calloc(grcnt+1, sizeof(int));

					for(int i=0; group[i]; i++) {
//						printf("Module %d is in group %d\n", i, group[i]);
						counts[group[i]]++;
					}

					int prod=1;
					for(int i=1; i<=grcnt; i++) {
						
						prod*=counts[i];
					}

					printf("\nFound %d groups having removed connections %d, %d and %d\nThe multiple is %d\n",
						grcnt,
						r0, r1, r2,
						prod);

					return 0;
				}

				tempConn[connection[r0][0]][connection[r0][1]] = 1;
				tempConn[connection[r0][1]][connection[r0][0]] = 1;
				tempConn[connection[r1][0]][connection[r1][1]] = 1;
				tempConn[connection[r1][1]][connection[r1][0]] = 1;
				tempConn[connection[r2][0]][connection[r2][1]] = 1;
				tempConn[connection[r2][1]][connection[r2][0]] = 1;
//				for(int iter=0; iter<MAXY; iter++) free(tempConn[iter]);
//				free(tempConn);
				free(group);
			}
		}
	}


//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)

	return 0;
}
