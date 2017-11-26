#include "boolean.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tokenizer.c"
#define CFG_File "CFG.txt"

typedef struct {
  char start[50];
  char end[256];
} production;

production *CFG_Prod; // Productions are stored here
int NProd;
Token *input; // Input (tokenized) are stored here
char ***table;
int N;

void TraverseTable(char *in, char *out) {
  /* Search if *in is on one of CFG_Prod.end
     If found, *out = CFG_Prod.start
     If multiple CFG_Prod.end found, *out will be a concatenation of all CFG_Prod.start, delimited by |
     If not found, *out = "" */
  int i;
  boolean IsFirst = true;
  out[0] = '\0';
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
  char temp1[256];
  strcpy(temp1,sub1);
  char **str1 = (char **) malloc (sizeof(char *));
  int i = 1;
  char *token;
  token = strtok(temp1,delim);
  while (token != NULL) {
    str1 = (char **) realloc(str1,(i+2) * sizeof(char *));
    str1[i] = (char *) malloc (256 * sizeof(char));
    strcpy(str1[i],token);
    token = strtok(NULL,delim);
    i++;
  }
  int Neff1 = i - 1;
  /* for (i = 1;i <= Neff1;i++) {
    printf("%s\n",str1[i]);
  } */
  char temp2[256];
  strcpy(temp2,sub2);
  char **str2 = (char **) malloc (sizeof(char *));
  i = 1;
  token = strtok(temp2,delim);
  while (token != NULL) {
    str2 = (char **) realloc(str2,(i+2) * sizeof(char *));
    str2[i] = (char *) malloc (256 * sizeof(char));
    strcpy(str2[i],token);
    token = strtok(NULL,delim);
    i++;
  }
  int Neff2 = i - 1;
  /* for (i = 1;i <= Neff2; i++) {
    printf("%s\n",str2[i]);
  } */
  int j;
  char temp[256];
  char tempout[256];
  out[0] = '\0';
  boolean isFirst = true;
  for (i = 1;i <= Neff1;i++) {
    for (j = 1;j <= Neff2;j++) {
      temp[0] = '\0';
      tempout[0] = '\0';
      strcat(temp,str1[i]);
      strcat(temp,str2[j]);
      TraverseTable(temp,tempout);
      if (!isFirst && (strlen(tempout) != 0)) {
        strcat(out," | ");
      }
      if (strlen(tempout) != 0) {
        isFirst = false;
      }
      strcat(out,tempout);
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
  int i = 0;
  /* FILE *CFGDump = fopen("cfg_dump.txt","w");
  for (i = 0;i <= NProd;i++) {
    fprintf(CFGDump,"%s -> %s\n",CFG_Prod[i].start,CFG_Prod[i].end);
  }
  fclose(CFGDump); */
  char fname[128], temp;
  printf("Input filename to be tested (max. 127 characters) : ");
  temp = getchar();
  while ((temp != '\n') && (i < 127)) {
    fname[i] = temp;
    i++;
    temp = getchar();
  }
  fname[i] = '\0';
  LoadTestFile(fname);
  printf("Detected %d terminals.\n",N);

  for (i = 1; i <= N;i++) {
    printf("%s ",Symbol(input[i]));
  }
  printf("\n");

  // Initialize parser table
  printf("Building empty table...\n");
  table = (char ***) malloc ((N+1) * sizeof(char **));
  int j;
  for (i = 1;i <= N;i++) {
    table[i] = (char **) malloc ((i+1) * sizeof(char *));
    for (j = 1;j <= i;j++) {
      table[i][j] = (char *) malloc (256 * sizeof(char));
    }
  }

  for (i = 1;i <= N;i++) {
    TraverseTable(Symbol(input[i]),table[N][i]);
  }
  /* for (i = 1;i <= N;i++) {
    printf("%d. %s\n",i,table[N][i]);
  } */

  char wordtemp[256];
  int d, k;
  for (i = N-1;i >= 1;i--) {
    for (j = 1;j <= i;j++) {
      d = 1;
      table[i][j][0] = '\0';
      for (k = 1;k <= (N-i);k++) {
        wordtemp[0] = '\0';
        SplitandTraverse(table[N-k+1][j],table[i+k][j+d],wordtemp);
        strcat(table[i][j],wordtemp);
        d++;
      }
    }
  }

  printf("%s\n",table[1][1]);
  FILE *debug = fopen("debugout.csv","w");
  for (i = 1;i <= N;i++) {
    fprintf(debug,"%s",table[i][1]);
    for (j = 2;j <= i;j++) {
      fprintf(debug,",%s",table[i][j]);
    }
    fprintf(debug,"\n");
  }
  fclose(debug);
  free(CFG_Prod);
  return 0;
}
