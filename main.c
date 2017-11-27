#include "boolean.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // Used only to measure time taken to parse
#include "tokenizer.c"
#define CFG_File "CFG.txt"

typedef struct {
  char start[50];
  char end[256];
} production; // Data struct for CNF productions

typedef struct tTableNode *address;
typedef struct tTableNode {
  address parent;
  char Tab[256];
} TableNode; // Data struct for the parser table

production *CFG_Prod; // Productions (in CNF) are stored here
int NProd; // Number of productions stored
Token *input; // Input (tokenized) are stored here
TableNode **table; // Parser table storage
int N; // Number of terminals detected

void UnDuplicate(char *in) {
  /* Removes duplicate string in *in
     String *in is in format : <str1> | <str2> | ... | <str-n> */
  char temp[256];
  int i;
  strcpy(temp,in);
  char **str = (char **) malloc (sizeof(char *));
  int Neff = 0;
  char *token;
  token = strtok(temp," | ");
  if (token != NULL) {
    str[Neff] = (char *) malloc (128 * sizeof(char));
    strcpy(str[Neff],token);
    Neff++;
    str = (char **) realloc (str,(Neff+1) * sizeof(char *));
    token = strtok(NULL," | ");
    while (token != NULL) {
      boolean found = false;
      i = 0;
      while ((i < Neff) && !found) {
        found = (strcmp(str[i],token) == 0);
        if (!found) i++;
      }
      if (!found) {
        str[Neff] = (char *) malloc (128 * sizeof(char));
        strcpy(str[Neff],token);
        Neff++;
        str = (char **) realloc (str,(Neff+1) * sizeof(char *));
      }
      token = strtok(NULL," | ");
    }
    in[0] = '\0';
    strcat(in,str[0]);
    for (i = 1;i < Neff;i++) {
      strcat(in," | ");
      strcat(in,str[i]);
    }
  }
}

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
     *sub1 and *sub2 both have string in this format : <str1> | <str2> | ... | <str-n>
     Puts the result in *out */
  // Stores delimiter
  char delim[4];
  strcpy(delim," | ");

  // Splits the first string
  char temp1[256]; // Temporary container of the string. This is needed because strtok will alter the original string
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
  int Neff1 = i - 1; // Sets the number of substring extracted

  // Splits the second string
  char temp2[256]; // Temporary container of the string. This is needed because strtok will alter the original string
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

  // Make any possible combinations from each substrings, and search
  // the production table. (using TraverseTable)
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
// Loads the CFG (in CNF) file into CFG_Prod
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
  LoadCFG(); // Loads the file that contain the CFG
  int i = 0;
  int j, k, d;
  clock_t start, end;
  double time_used;
  char fname[128], temp;
  // Asks the file to be evaluated
  printf("Input filename to be tested (max. 127 characters) : ");
  temp = getchar();
  while ((temp != '\n') && (i < 127)) {
    fname[i] = temp;
    i++;
    temp = getchar();
  }
  fname[i] = '\0';
  if(!LoadTestFile(fname)) { // Trying to load given file
    printf("File not found!\n");
  }
  else {
    printf("Detected %d terminals.\n",N);

    // Initialize parser table
    printf("Building empty table...\n");
    table = (TableNode **) malloc ((N+1) * sizeof(TableNode *));
    for (i = 1;i <= N;i++) {
      table[i] = (TableNode *) malloc ((i+1) * sizeof(TableNode));
    }

    // Starts to parse the file
    printf("Parsing...\n");
    start = clock();
    // Fill up the bottom row
    for (i = 1;i <= N;i++) {
      TraverseTable(Symbol(input[i]),table[N][i].Tab);
    }

    // Fill up the next rows, up until the first row
    char wordtemp[256];
    boolean isFirst = true;
    for (i = N-1;i >= 1;i--) {
      for (j = 1;j <= i;j++) {
        d = 1;
        table[i][j].Tab[0] = '\0';
        for (k = 1;k <= (N-i);k++) {
          wordtemp[0] = '\0';
          SplitandTraverse(table[N-k+1][j].Tab,table[i+k][j+d].Tab,wordtemp);
          if (isFirst && (strlen(wordtemp) != 0)) {
            strcat(table[i][j].Tab,wordtemp);
            isFirst = false;
            table[N-k+1][j].parent = &table[i][j];
            table[i+k][j+d].parent = &table[i][j];
          }
          else if (strlen(wordtemp) != 0) {
            strcat(table[i][j].Tab," | ");
            strcat(table[i][j].Tab,wordtemp);
            table[N-k+1][j].parent = &table[i][j];
            table[i+k][j+d].parent = &table[i][j];
          }
          d++;
        }
        UnDuplicate(table[i][j].Tab);
      }
    }
    end = clock();
    time_used = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Parse complete in %f second(s)\n",time_used);

    // Checks if the file is accepted/rejected
    if (strcmp(table[1][1].Tab,"<START>") == 0) {
      printf("Program accepted!\n");
    }
    else {
      printf("Program rejected!\n");
      // Try to search the problem
      boolean found = false;
      for (i = N;(i >= 1) && !found;i--) {
        if (table[N][i].parent == NULL) {
          printf("Syntax error detected in line %d.\n",Line(input[i]));
          found = true;
        }
      }
    }

    // Dumps the parser table into debugout.csv
    FILE *debug = fopen("debugout.csv","w");
    for (i = 1;i <= N;i++) {
      fprintf(debug,"%s",table[i][1].Tab);
      for (j = 2;j <= i;j++) {
        fprintf(debug,",%s",table[i][j].Tab);
      }
      fprintf(debug,"\n");
    }
    fprintf(debug,"%s",Symbol(input[1]));
    for (i = 2;i <= N;i++) {
      fprintf(debug,",%s",Symbol(input[i]));
    }
    printf("Generated parser table stored at debugout.csv\n");
    fclose(debug);
    free(table);
  }
  free(CFG_Prod);
  return 0;
}
