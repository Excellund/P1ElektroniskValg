
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>
#include <unistd.h>

/* Constants */
#define TRUE 1
#define FALSE 0
#define CPR_LEN 10
#define DATA_LEN 4
#define EXIT_SUCCES 0

/* Prototypes */
int verifyIdentity(char *);
int isLeapYear(char *, char);
int BinarySearch(FILE **, char *, int *);
void moveFileText(FILE **cp, int position, char *CPR);

/* Main Function */
int main(void) {
  char CPR[CPR_LEN+1], person[2], party[2], data[DATA_LEN+1];
  int position = 0;

  FILE *cp = fopen("identification", "r+");
  FILE *rp = fopen("results", "r+");

  while (TRUE) {
    printf("Indtast venligst dit CPR-nummer: ");
    strcpy(CPR, "");

    scanf(" %s", CPR);

    if (!verifyIdentity(CPR)) {
      printf("Incorrect ID.\n");
    } else {
      if (BinarySearch(&cp, CPR, &position)) {
        printf("You have already voted and aren't allowed to vote again.\n");
      } else {
        moveFileText(&cp, position, CPR);

        printf("Angiv venligst din stemme for parti og person: ");
        scanf(" %[A-Z]", party);
        scanf(" %[A-Z]", person);

        strcpy(data, "");
        strcat(data, party);
        strcat(data, ", ");
        strcat(data, person);

        fseek(rp, 0, SEEK_END);
        fprintf(rp, "%s\n", data);

        return EXIT_SUCCES;
      }
    }
  }

  fclose(cp);
  fclose(rp);
  return EXIT_SUCCES;
}

int verifyIdentity(char *CPR) {

  char CPR_Splittet[4][5];
  int i = 0;

  if (strlen(CPR) > CPR_LEN) {
    return FALSE;
  }

  for (i = 0; i < CPR_LEN - 2 ; i += 2) {
    strncpy(CPR_Splittet[i/2], CPR + i, i == 6 ? 4 : 2);
    CPR_Splittet[i/2][i == 6 ? 4 : 2] = 0;
  }

  for (i = 0; i < 12; i++) {
    if (atoi(CPR_Splittet[1]) > 12 || atoi(CPR_Splittet[1]) < 1) {
      return FALSE;
    } else if (atoi(CPR_Splittet[1]) == 2) {
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

void moveFileText(FILE **cp, int position, char *CPR) {
  char c;
  FILE *cp_temp = fopen("cpr_temp", "w+");
  fseek(*cp, position, SEEK_SET);
  while ((c = getc(*cp)) != EOF) {
    putc(c, cp_temp);
  }
  fseek(*cp, position, SEEK_SET);

  fwrite(CPR, 1, strlen(CPR), *cp);
  fseek(*cp, position + 12, SEEK_SET);
  fseek(cp_temp, 0, SEEK_SET);

  while ((c = getc(cp_temp)) != EOF) {
    putc(c, *cp);
  }
  fclose(cp_temp);
}
