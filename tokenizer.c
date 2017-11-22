#include <stdio.h>
#include <string.h>
#include "token.h"
#include "token.c"
 
struct Token
{
     char symbol[3];
     char word[20];
     int line;
}record[10000];
 
int main() {
	int i, Neff = 0;
	FILE *fp;
    fp = fopen("test.txt","r");
    
    while (!(feof(fp))) {
		fscanf(fp, "%s", record[Neff].word);
		//record[Neff].line = Neff;
		Neff++;
	}
	
	for (i = 0; i < Neff; i++) {
		puts(record[i].word);
		//printf("%d\n", record[i].line);
	}
	
	return 0;
}
