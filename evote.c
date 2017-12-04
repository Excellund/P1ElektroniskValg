#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

/* Constants */
#define TRUE 1
#define FALSE 0
#define CPR_LEN 10
#define EXIT_SUCCES 0

/* Prototypes */
int verifyIdentity(char *);
void RemoveCharacter(char *, char);
int isLeapYear(char *, char);

/* Main Function */
int main(void) {

  char CPR[CPR_LEN];
  int CPR_int, CPR_check, lines;

  FILE *cp = fopen("cpr", "r+");

  printf("Indtast venligst dit CPR-nummer: ");
  scanf("%[0-9]", CPR);

  if (!verifyIdentity(CPR)) {
    printf("Incorrect ID.\n");
  }

  CPR_int = atoi(CPR);

  fseek(cp, 0L, SEEK_END);
  lines = ftell(cp)/12;

  /*while (fscanf(cp, "%d \n", &CPR_check) != EOF) {
    if (CPR_int == CPR_check) {

    }
  }*/

  fclose(cp);
  return EXIT_SUCCES;
}

int verifyIdentity(char *CPR) {

  char CPR_Splittet[4][5];
  int i = 0;

  for (i = 0; i < CPR_LEN - 2 ; i += 2) {
    strncpy(CPR_Splittet[i/2], CPR + i, i == 6 ? 4 : 2);
    CPR_Splittet[i/2][i == 6 ? 4 : 2] = 0;
  }

  for (i = 0; i < 12; i++) {
    if (atoi(CPR_Splittet[1]) == 2) {
      if (atoi(CPR_Splittet[0]) > 28 + isLeapYear(CPR_Splittet[2], CPR_Splittet[3][0])) {
        return FALSE;
      }
    }
    else {
      if (atoi(CPR_Splittet[1]) == i) {
        if (atoi(CPR_Splittet[0]) > 30 + ((i <= 7) ? (i % 2) : ((i - 1) % 2))) {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

int isLeapYear(char * yearLastDigits, char ciffer) {
  char yearString[4];
  int cifferInt;
  int yearLastDigitsInt;
  int year;

  yearLastDigitsInt = atoi(yearLastDigits);
  cifferInt = ciffer - '0';

  if (cifferInt >= 5 && cifferInt <= 8 && yearLastDigitsInt >= 58) {
    strcpy(yearString, "18");
  } else if (cifferInt <= 3 || ((cifferInt == 4 || cifferInt == 9) && yearLastDigitsInt >= 37)) {
    strcpy(yearString, "19");
  } else {
    strcpy(yearString, "20");
  }

  strcpy(yearString + 2, yearLastDigits);
  year = atoi(yearString);

  if ( (year % 4 == 0) && ( year % 100 != 0 || year % 400 == 0 ))
    return 1;
  else
    return 0;
}

/* Maybe not necassary? */
void RemoveCharacter(char* source, char character)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != character)
      i++;
  }
  *i = 0;
}
