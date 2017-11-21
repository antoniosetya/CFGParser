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
Token *input; // Input (tokenized) are stored here

int main() {
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
  i--;
  printf("\nDone reading the file.\n");
  fclose(GFile);
  /* printf("Input the file to be evaluated : ");scanf("%s",temp);
  FILE *InFile = fopen(temp,"r");

  fclose(InFile); */
  free(CFG_Prod);
  return 0;
}
