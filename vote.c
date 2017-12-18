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
#define MONTHS_IN_YEAR 12
#define NEWLINE_SIZE 2
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

  /* Initialize file pointers in read and write mode */
  FILE *cp = fopen("identification", "r+");
  FILE *rp = fopen("results", "r+");


  while (TRUE) {

    /* Promt for identification */
    printf("Indtast venligst dit CPR-nummer: ");
    strcpy(CPR, "");
    scanf(" %s", CPR);

    /* check identity authenticity */
    if (!verifyIdentity(CPR)) {
      printf("Incorrect ID.\n");
    } else {

      /* Checker whether the user has voted earlier. */
      if (BinarySearch(&cp, CPR, &position)) {
        printf("You have already voted and aren't allowed to vote again.\n");
      } else {
        moveFileText(&cp, position, CPR);

        /* Promt for voting data */
        printf("Angiv venligst din stemme for parti og person: ");
        scanf(" %[A-Z]", party);
        scanf(" %[A-Z]", person);

        /* Collects the inputs in a single data variable */
        strcpy(data, "");
        strcat(data, party);
        strcat(data, ", ");
        strcat(data, person);

        /* Print the data to the results file */
        fseek(rp, 0, SEEK_END);
        fprintf(rp, "%s\n", data);

        return EXIT_SUCCES;
      }
    }
  }

  /* Close opened files */
  fclose(cp);
  fclose(rp);
  return EXIT_SUCCES;
}

/* Function that checks if the identification is valid */
int verifyIdentity(char *CPR) {

  char CPR_Splittet[4][5];
  int i = 0;

  /* Check if the input was too long */
  if (strlen(CPR) > CPR_LEN) {
    return FALSE;
  }

  /* Split the CPR in it's parts */
  for (i = 0; i < CPR_LEN - 2 ; i += 2) {
    strncpy(CPR_Splittet[i/2], CPR + i, i == 6 ? 4 : 2);
    CPR_Splittet[i/2][i == 6 ? 4 : 2] = 0;
  }

  /* For all the months in a year */
  for (i = 0; i < MONTHS_IN_YEAR; i++) {

    /* If the amount is not within the interval */
    if (atoi(CPR_Splittet[1]) > MONTHS_IN_YEAR || atoi(CPR_Splittet[1]) < 1) {
      return FALSE;

    /* if the month is february */
    } else if (atoi(CPR_Splittet[1]) == 2) {
      /* Check if the day is under the amount of days that year */
      if (atoi(CPR_Splittet[0]) > 28 + isLeapYear(CPR_Splittet[2], CPR_Splittet[3][0])) {
        return FALSE;
      }
    }
    /* For all the other months */
    else {
      /* Find the correct month */
      if (atoi(CPR_Splittet[1]) == i) {
        /* Check how many days are in the month and check if the input is valid */
        if (atoi(CPR_Splittet[0]) > 30 + ((i <= 7) ? (i % 2) : ((i - 1) % 2))) {
          return FALSE;
        }
      }
    }
  }

  /* If everything is ok, return TRUE */
  return TRUE;
}

/* Function data returns a boolean value to determine if a year is a leapyear */
int isLeapYear(char * yearLastDigits, char ciffer) {
  char yearString[4];
  int cifferInt;
  int yearLastDigitsInt;
  int year;

  /* Convert the chars to int */
  yearLastDigitsInt = atoi(yearLastDigits);
  cifferInt = ciffer - '0';

  /* Determine the century */
  if (cifferInt >= 5 && cifferInt <= 8 && yearLastDigitsInt >= 58) {
    strcpy(yearString, "18");
  } else if (cifferInt <= 3 || ((cifferInt == 4 || cifferInt == 9) && yearLastDigitsInt >= 37)) {
    strcpy(yearString, "19");
  } else {
    strcpy(yearString, "20");
  }

  /* Get the resulting year */
  strcpy(yearString + 2, yearLastDigits);
  year = atoi(yearString);

  /* Check for leapyear */
  if ( (year % 4 == 0) && ( year % 100 != 0 || year % 400 == 0 ))
    return 1;
  else
    return 0;
}

/* Performs a binary search to find out if the CPR already has been used to vote */
int BinarySearch(FILE **cp, char *CPR, int *position) {
  int current_line, upper, lower = 0;
  double CPR_number, CPR_check;
  char CPR_check_string[CPR_LEN];

  /* Stores the CPR as a double */
  sscanf(CPR, "%lf", &CPR_number);

  /* Determines the upper point */
  fseek(*cp, 0L, SEEK_END);
  upper = ftell(*cp) - (CPR_LEN + NEWLINE_SIZE);

  /* While still searching */
  while (lower <= upper) {

    /* Caclulate the middle line */
    current_line = (lower + (upper - lower) / 2) + (lower + (upper - lower) / 2) % (CPR_LEN + NEWLINE_SIZE);

    /* Position the file pointer */
    fseek(*cp, current_line, SEEK_SET);

    /* Scan the CPR */
    fscanf(*cp, "%[0-9]", CPR_check_string);

    /* Convert this CPR to a double aswell */
    sscanf(CPR_check_string, "%lf", &CPR_check);

    /* if the CPRs are equivalent */
    if (CPR_number == CPR_check) {
      *position = current_line;
      return TRUE;

    /* if they aren't */
    } else {
      /* Check if the original CPR value is 'less' than the checked one */
      if (CPR_number < CPR_check) {
        /* reposition the upper index */
        upper = current_line - (CPR_LEN + NEWLINE_SIZE);
      /* Check if the original CPR value is 'larger' than the checked one */
      } else if (CPR_number > CPR_check) {
        /* reposition the lower index */
        lower = current_line + (CPR_LEN + NEWLINE_SIZE);
        if (lower >= upper) {
          current_line += (CPR_LEN + NEWLINE_SIZE);
        }
      }
    }
  }

  /* Set the position */
  *position = current_line;

  return FALSE;
}

/* Function that uses a temporary file to move data instead of overwriting it */
void moveFileText(FILE **cp, int position, char *CPR) {
  char c;

  /* Open temporary file */
  FILE *cp_temp = fopen("cpr_temp", "w+");
  fseek(*cp, position, SEEK_SET);

  /* Write all data after the position to a temporary file */
  while ((c = getc(*cp)) != EOF) {
    putc(c, cp_temp);
  }
  fseek(*cp, position, SEEK_SET);

  /* Write the CPR at the desired position */
  fwrite(CPR, 1, strlen(CPR), *cp);

  /* Reposition pointers */
  fseek(*cp, position + (CPR_LEN + NEWLINE_SIZE), SEEK_SET);
  fseek(cp_temp, 0, SEEK_SET);

  /* Write everything from the temporary file, back in the original file again */
  while ((c = getc(cp_temp)) != EOF) {
    putc(c, *cp);
  }

  /* Close the file */
  fclose(cp_temp);
}
