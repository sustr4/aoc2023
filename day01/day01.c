#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Read input file line by line (e.g., into an array)
int *readInput() {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;
	int val;
	int sum=0;

	input = fopen("input.txt", "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		int last=-1;
		int first=-1;

		for(int i=0; i<strlen(line); i++) {
			val=-1;
			if((line[i]>='1') && (line[i]<='9')) val=line[i]-'0';
			if(!strncmp(line+i,"one",3)) val=1;
			if(!strncmp(line+i,"two",3)) val=2;
			if(!strncmp(line+i,"three",5)) val=3;
			if(!strncmp(line+i,"four",4)) val=4;
			if(!strncmp(line+i,"five",4)) val=5;
			if(!strncmp(line+i,"six",3)) val=6;
			if(!strncmp(line+i,"seven",5)) val=7;
			if(!strncmp(line+i,"eight",5)) val=8;
			if(!strncmp(line+i,"nine",4)) val=9;

			if(val>0) {
				if(first<0) {
					first=val;
					last=first;
				}
				else last=val;
			}
		}

		if(first>0) {
			printf("%d%d\t%s\n", first, last, line);
			sum+=first*10+last;
		}

		count++;
	}

	printf("%d\n", sum);

	fclose(input);
	if (line)
	free(line);


	return 0;
}

int main(int argc, char *argv[]) {

	readInput();

	return 0;
}
