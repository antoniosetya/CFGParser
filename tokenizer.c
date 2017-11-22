#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "boolean.h"

Token input[256]; // Input (tokenized) are stored here

int main() {
	FILE *fp;
	char ch;
	char tempword[128];
	tempword[0] = '\0';
	fp = fopen("test.txt", "r");
	int N = 1;
	int i = 0;
	ch = fgetc(fp);
	while(ch != EOF) {
		boolean KeepChar = (ch != ';') && (ch != ',') && (ch != '.') && (ch != ':') && (ch != '"');
		KeepChar = KeepChar && (ch != '+') && (ch != '-') && (ch != '*') && (ch != '/') && (ch != '=') && (ch != '>') && (ch != '<');
		KeepChar = KeepChar && (ch != '[') && (ch != ']') && (ch != '(') && (ch != ')');
		KeepChar = KeepChar && (ch != ' ') && (ch != '\n') && (ch != '\0') && (ch != EOF);
		while(KeepChar) {
			tempword[i] = ch;
			i++;
			ch = fgetc(fp);
			KeepChar = (ch != ';') && (ch != ',') && (ch != '.') && (ch != ':') && (ch != '"');
			KeepChar = KeepChar && (ch != '+') && (ch != '-') && (ch != '*') && (ch != '/') && (ch != '=') && (ch != '>') && (ch != '<');
			KeepChar = KeepChar && (ch != '[') && (ch != ']') && (ch != '(') && (ch != ')');
			KeepChar = KeepChar && (ch != ' ') && (ch != '\n') && (ch != '\0') && (ch != EOF);
		}
		tempword[i] = '\0';
		if (strlen(tempword) != 0) {
			strcpy(String(input[N-1]),tempword);
			assign_symbol(&input[N-1]);
			N++;
		}
		if ((ch != ' ') && (ch != '\n')) {
			tempword[0] = ch;
			tempword[1] = '\0';
			strcpy(String(input[N-1]),tempword);
			assign_symbol(&input[N-1]);
			N++;
		}
		i = 0;
		if (ch != EOF) {
			ch = fgetc(fp);
		}
	}
	fclose(fp);
	for (i = 0;i < N;i++) {
		printf("%s ",Symbol(input[i]));
	}
	return 0;
}
