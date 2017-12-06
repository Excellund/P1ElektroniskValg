#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

/* Constants */
#define TRUE 1
#define FALSE 0
#define CPR_LEN 10
#define EXIT_SUCCES 0

/* Prototypes */
int verifyIdentity(char *);
int isLeapYear(char *, char);
int BinarySearch(FILE **, char *, int *);
int getPower(double);
void RemoveCharacter(char *, char);

/* Main Function */
int main(void) {

  char c, CPR[CPR_LEN];
  int position = 0;

  FILE *cp_temp = fopen("cpr_temp", "w+");
  FILE *cp = fopen("cpr", "r+");

  printf("Indtast venligst dit CPR-nummer: ");
  scanf("%[0-9]", CPR);

  if (!verifyIdentity(CPR)) {
    printf("Incorrect ID.\n");
  }

  if (BinarySearch(&cp, CPR, &position)) {
    printf("You have already voted and aren't allowed to vote again.\n");
  } else {
    fseek(cp, position, SEEK_SET);
    while ((c = getc(cp)) != EOF) {
      putc(c, cp_temp);
    }
    fseek(cp, position, SEEK_SET);
    fwrite(CPR, 1, sizeof(CPR), cp);
    fseek(cp, position + 12, SEEK_SET);
    fseek(cp_temp, 0, SEEK_SET);
    while ((c = getc(cp_temp)) != EOF) {
      putc(c, cp);
    }
  }

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

int BinarySearch(FILE **cp, char *CPR, int *position) {

  int current_line, upper, lower = 0;
  double CPR_number, CPR_check;
  char CPR_check_string[CPR_LEN];

  sscanf(CPR, "%lf", &CPR_number);

  fseek(*cp, 0L, SEEK_END);
  upper = ftell(*cp)-12;

  while (lower <= upper) {
    current_line = (lower + (upper - lower) / 2) + (lower + (upper - lower) / 2) % 12;

    fseek(*cp, current_line, SEEK_SET);

    fscanf(*cp, "%[0-9]", CPR_check_string);

    sscanf(CPR_check_string, "%lf", &CPR_check);

    printf("%d %lf %lf %d %d\n", current_line, CPR_number, CPR_check, lower, upper);

    if (CPR_number == CPR_check) {
      *position = current_line;
      return TRUE;
    } else {
      if (CPR_number < CPR_check) {
        upper = current_line - 12;
      } else if (CPR_number > CPR_check) {
        lower = current_line + 12;
        if (lower >= upper) {
          current_line += 12;
        }
      }
    }
  }

  *position = current_line;

  return FALSE;
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

int getPower(double n) {
  int count = 0;

  while (n >= 2) {
    n /= 2;
    count++;
  }

  return count;
}
