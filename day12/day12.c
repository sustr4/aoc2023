#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

long total=0;

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 35
#define MAXY 1001
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	char *map;
	long vars; // Possible variations based on ?
	int *ops;
	int maxop;
	int has;
	int needs;
	int ques;
	int *mustremain;
} TLine;

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

// Retrieve nth neighbor from a map
int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1};
int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1};
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<8));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
}

void multiply(int *vars) {
	int l;
	for(l=0; vars[l]; l++);

	for(int i=0; i<l; i++) {
		for(int y=1; y<5; y++) {
			vars[y*l+i]=vars[i];
		}
	}
}

// Read input file line by line (e.g., into an array)
TLine *readInput() {
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
	TLine *inst=(TLine*)calloc(MAXY, sizeof(TLine));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	// char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) continue;

		inst[count].ops=calloc(MAXX, sizeof(int));

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		asprintf(&(inst[count].map), "%s?%s?%s?%s?%s", token, token, token, token, token);
		for(int ii=0; ii<=strlen(inst[count].map); ii++) {
			if (inst[count].map[ii]=='#') inst[count].has++;
			if (inst[count].map[ii]=='?') inst[count].ques++;
		}
		int y=0;
		while( 1 ) {
			if(!(token = strtok(NULL, ","))) break;
			inst[count].ops[y] = atoi(token);
			inst[count].needs+=inst[count].ops[y];
			y++;
		}
		multiply(inst[count].ops);
		inst[count].needs*=5;
		for(inst[count].maxop=0; inst[count].ops[inst[count].maxop]; inst[count].maxop++);

		for(int j=0; j<inst[count].maxop; j++) {
			inst[count].mustremain=-1;
			for(int k=j; j<inst[count].maxop; j++) {
				inst[count].mustremain+=1+inst[count].ops[k];
			}
		}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}


void countVars(TLine *array) {
	for(int i=0; array[i].map; i++) {
		array[i].vars=1;
		for(int y=0; y<strlen(array[i].map); y++) {
			if(array[i].map[y]=='?') array[i].vars=array[i].vars << 1;
		}
	}
}

char *variation(int num, TLine line) {

	if(num>=line.vars) return NULL;

	char *map=strdup(line.map);
	int q=-1;
	for(int i=0; i<strlen(map); i++) {
		if(map[i]=='?') {
			q++;
			int bit=1<<q;
			if(num & bit) map[i]='#';
			else map[i]='.';
		}
	}

	return map;
}

int *sum(char *map) {

	int *sum = calloc(MAXX, sizeof(int));
	int inq = 0;
	int y = -1;

	for(int i=0; i<strlen(map); i++) {
		if(map[i]=='#') {
			if(inq) sum[y]++;
			else {
				sum[++y]=1;
				inq=1;
			}
		}
		else inq=0;
	}

	return sum;
}

int compare(int *a, int *b) {

	int la, lb;
	for(la=0; a[la]; la++);
	for(lb=0; b[lb]; lb++);

	if(la!=lb) return 0;

	for(int y=0; y<=la; y++) {
		if(a[y]!=b[y]) return 0;
	}
	return 1;
}

int next(char* map, int op, int maxop, int *ops) {


//	printf("%90s (%2ld) (%d/%d: %d)\n", map, strlen(map), op, maxop, ops[op]);

	if(map[0]==0) {
		if(op==maxop) total++;
		// TODO: This will need some cheking
		return 0;
	}

	if(op==maxop) { // operational vents used up. Only empties must remain
		int k=0;
		while(map[k]) if(map[k++]=='#') return 0;
	} 

	if(map[0]=='.') next(map + 1, op, maxop, ops);

	if(map[0]=='#') {
//		printf("Yes, the hash is reconized\n");
		if(strlen(map)<ops[op]) return 0; // map too short
//		printf("and not too short\n");
		int y;
		for(y=0; y<ops[op]; y++) {
			if(map[y]=='.') return 0; // there is a gap in the run
		}
//		printf("Run for %d, next character is %c.\n",y,map[y]);
		if(map[y]=='.')		next(map+y+1, op+1, maxop, ops); // Skip safely the border character
		else if(map[y]=='?')	next(map+y+1, op+1, maxop, ops); // Skip safely the border character
		else if(map[y]==0)	next(map+y, op+1, maxop, ops); // This will be the final check

		return 0; // The run is too long. A dot or end of string must follow
	}
	
	if(map[0]=='?') {
		map[0]='#';
		next(map, op, maxop, ops); // Like '#'
		map[0]='?';

		next(map+1, op, maxop, ops); // Like '.'
	}

	return 0;
} 

int main(int argc, char *argv[]) {

	TLine *array = readInput();
	countVars(array);
	int i=0;	
//	array = readInput();

/*	for(i=0; array[i].map; i++) {
		printf("l%04d:\n\t%s\n\t", i, array[i].map);
		for(int y=0; array[i].ops[y]; y++) printf("%d, ", array[i].ops[y]);
		printf("\n\t=> %ld variations\n", array[i].vars);
		long long perm = 1;
		for(int ii=array[i].needs - array[i].has +1; ii<=array[i].ques; ii++) perm*=ii;
		printf("\t.. has %d hashes, needs %d, therefore misses %d in %d places => %lld permutations.\n", array[i].has, array[i].needs, array[i].needs - array[i].has, array[i].ques, perm);
	}*/


	#pragma omp parallel for shared(total)
	for(int line=0; line<MAXY; line++) {
		if(!array[line].map) continue;
		next(array[line].map, 0, array[line].maxop, array[line].ops);
		printf("%4d:\t%ld\n", line, total);
	}

	printf("Total: %ld\n", total);

	return 0; // TODO Fix adn remove


	#pragma omp parallel for shared(total)
	for(int line=0; line<MAXY; line++) {
		if(!array[line].map) continue;
		printf("Line %d (total so far %ld)\n", line, total);
		i=0;
		while (1) {

			char *v=variation(i++, array[line]);
			if(!v) break;


			int *s=sum(v);
			if(compare(s, array[line].ops)) {
//				printf("l%04d:\t%3d: %s\t", line, i, v);
//				for(int y=0; s[y]; y++) printf("%d, ", s[y]);
				total++;
//				printf(" match %ld!\n", total);
			}


			free(v);
			free(s);
		}
	}
	printf("Total: %ld\n", total);

	return 0;
}
