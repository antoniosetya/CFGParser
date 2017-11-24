#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "boolean.h"

extern Token *input; // Input (tokenized) are stored here
extern int N;

Token *input;
int N = 1;

void LoadTestFile(char *filename) {
	FILE *fp;
	char ch, ch2;
	ch2 = '\0';
	char tempword[128];
	tempword[0] = '\0';
	fp = fopen(filename, "r");
	input = (Token *) malloc (2 * sizeof(Token));
	int i = 0;
	ch = fgetc(fp);
	while(ch != EOF) {
		boolean KeepChar = (ch != ';') && (ch != ',') && (ch != '.') && (ch != ':') && (ch != '"');
		KeepChar = KeepChar && (ch != '+') && (ch != '-') && (ch != '*') && (ch != '/') && (ch != '=') && (ch != '>') && (ch != '<');
		KeepChar = KeepChar && (ch != '[') && (ch != ']') && (ch != '(') && (ch != ')');
		KeepChar = KeepChar && (ch != ' ') && (ch != '\n') && (ch != '\0') && (ch != '\t') && (ch != EOF);
		while(KeepChar) {
			tempword[i] = ch;
			i++;
			ch = fgetc(fp);
			KeepChar = (ch != ';') && (ch != ',') && (ch != '.') && (ch != ':') && (ch != '"');
			KeepChar = KeepChar && (ch != '+') && (ch != '-') && (ch != '*') && (ch != '/') && (ch != '=') && (ch != '>') && (ch != '<');
			KeepChar = KeepChar && (ch != '[') && (ch != ']') && (ch != '(') && (ch != ')');
			KeepChar = KeepChar && (ch != ' ') && (ch != '\n') && (ch != '\0') && (ch != '\t') && (ch != EOF);
		}
		tempword[i] = '\0';
		if (strlen(tempword) != 0) {
			strcpy(String(input[N]),tempword);
			N++;
			input = (Token *) realloc (input,((N+2) * sizeof(Token)));
		}
		if ((ch != ' ') && (ch != '\n') && (ch != '\t')) {
			ch2 = fgetc(fp);
			if ((ch == ':') && (ch2 == '=')) {
				strcpy(String(input[N]),":=");
				ch2 = '\0';
			}
			else if ((ch == '<') && (ch2 == '=')) {
				strcpy(String(input[N]),"<=");
				ch2 = '\0';
			}
			else if ((ch == '<') && (ch2 == '>')) {
				strcpy(String(input[N]),"<>");
				ch2 = '\0';
			}
			else if ((ch == '>') && (ch2 == '=')) {
				strcpy(String(input[N]),">=");
				ch2 = '\0';
			}
			else if ((ch == '.') && (ch2 == '.')) {
				strcpy(String(input[N]),"..");
				ch2 = '\0';
			}
			else {
				tempword[0] = ch;
				tempword[1] = '\0';
				strcpy(String(input[N]),tempword);
				ch = ch2;
			}
			N++;
			input = (Token *) realloc (input,((N+2) * sizeof(Token)));
		}
		i = 0;
		if ((ch != EOF) && (ch2 == '\0')) {
			ch = fgetc(fp);
		}
		ch2 = '\0';
	}
	fclose(fp);
	N--;
	for (i = 1;i <= N;i++) {
		assign_symbol(&input[i]);
	}
}
