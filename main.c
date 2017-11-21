#include "boolean.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CFG_File "CFG.txt"

typedef struct {
  char start[50];
  char end[256];
} production;

production *CFG_Prod; // Productions are stored here
int NProd;
Token *input; // Input (tokenized) are stored here
char ***table;

void TraverseTable(char *in, char *out) {
  /* Search if *in is on one of CFG_Prod.end
     If found, *out = CFG_Prod.start
     If multiple CFG_Prod.end found, *out will be a concatenation of all CFG_Prod.start, delimited by |
     If not found, *out = "" */
  int i;
  boolean IsFirst = true;
  for (i = 0;i <= NProd; i++) {
    if (strcmp(CFG_Prod[i].end,in) == 0) {
      if (!IsFirst) {
        strcat(out," | ");
      }
      else {
        IsFirst = false;
      }
      strcat(out,CFG_Prod[i].start);
    }
  }
}

void SplitandTraverse(char *sub1, char *sub2, char *out) {
  /* Splits, remake, and traverse CFG_Prod of any non-terminal combinations found on *sub1 and *sub2
     Puts the result in *out */
  char delim[3] = " | ";
  char **str1 = (char **) malloc (sizeof(char *));
  int i = 1;
  str1[i] = (char *) malloc (256 * sizeof(char));
  char *token;
  token = strtok(sub1,delim);
  while (token != NULL) {
    str1 = (char **) realloc(str1,i+1);
    str1[i] = (char *) malloc (256 * sizeof(char));
    strcpy(str1[i],token);
    token = strtok(NULL,delim);
    i++;
  }
  int Neff1 = i--;
  char **str2 = (char **) malloc (sizeof(char *));
  i = 1;
  token = strtok(sub2,delim);
  while (token != NULL) {
    str2 = (char **) realloc(str1,i+1);
    str2[i] = (char *) malloc (256 * sizeof(char));
    strcpy(str2[i],token);
    token = strtok(NULL,delim);
    i++;
  }
  int Neff2 = i--;
  int j;
  char temp[512];
  for (i = 0;i <= Neff1;i++) {
    for (j = 0;j <= Neff2;j++) {

    }
  }
}

void LoadCFG() {
// Loads the CFG file
  CFG_Prod = (production *) malloc (sizeof(production));
  printf("Reading grammar file... ");
  FILE *GFile = fopen(CFG_File,"r");
  int i = 0;
  char temp[256];
  boolean fileValid = true;
  int scancode = fscanf(GFile,"%s",temp);
  while ((scancode == 1) && fileValid) {
    strcpy(CFG_Prod[i].start,temp);
    scancode = fscanf(GFile,"%s",temp);
    if (scancode == 1) {
      if(strcmp(temp,"->") == 0) {
        scancode = fscanf(GFile,"%s",temp);
        if (scancode == 1) {
          strcpy(CFG_Prod[i].end,temp);
          i++;
          CFG_Prod = (production *) realloc (CFG_Prod,(i+1) * sizeof(production));
          scancode = fscanf(GFile,"%s",temp);
          while((scancode == 1) && (strcmp(temp,"|") == 0)) {
            scancode = fscanf(GFile,"%s",temp);
            if (scancode == 1) {
              strcpy(CFG_Prod[i].start,CFG_Prod[i-1].start);
              strcpy(CFG_Prod[i].end,temp);
              i++;
              CFG_Prod = (production *) realloc (CFG_Prod,(i+1) * sizeof(production));
              scancode = fscanf(GFile,"%s",temp);
            }
            else {
              fileValid = false;
              printf("\nWrong file format detected! Please fix the format first!\n");
            }
          }
        }
        else {
          fileValid = false;
          printf("\nWrong file format detected! Please fix the format first!\n");
        }
      }
      else {
        fileValid = false;
        printf("\nWrong file format detected! Please fix the format first!\n");
      }
    }
    else {
      fileValid = false;
      printf("\nWrong file format detected! Please fix the format first!\n");
    }
  }
  printf("\nDone reading the file.\n");
  NProd = i-1;
  printf("Productions read : %d\n",NProd);
  fclose(GFile);
}

int main() {
  LoadCFG();
  /* printf("Input the file to be evaluated : ");scanf("%s",temp);
  FILE *InFile = fopen(temp,"r");
  // Initialize parser table
  table = (char ***) malloc (N * sizeof(char *));
  int j;
  for (i = 1;i <= N;i++) {
    table[i] = (char **) malloc (i * sizeof(char));
    for (j = 1;j <= i;j++) {
      table[i][j] = (char *) malloc (256 * sizeof(char));
    }
  }

  for (i = 1;i <= N;i++) {
    TraverseTable(input[i],table[N][i]);
  }

  for (i = N-1;i >= 1;i--) {
    for (j = 1;j <= i;j++) {

    }
  }
  */

  // fclose(InFile);
  free(CFG_Prod);
  return 0;
}
