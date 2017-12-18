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

/*Prototypes*/
void Menu1(int, char CPR[CPR_LEN + 1]);
void MenuPartyList();
int verifyIdentity(char *);
int isLeapYear(char *, char);
int binarySearch(FILE **, char *, int *);
void moveFileText(FILE **, int, char *);
void SocialDemokratiet();

int main(void)
{
  int id = 0;
  char CPR[CPR_LEN + 1];
  Menu1(id, CPR);

  return EXIT_SUCCES;
}

void Menu1(int id, char CPR[DATA_LEN + 1]){
  char person[2], party[2], data[DATA_LEN+1];
  int position = 0;
  int choice;
  int * pId = &id;

  /* Initialize file pointers in read and write mode */
  FILE *cp = fopen("identification", "r+");
  FILE *rp = fopen("results", "r+");

  /*Displaing on screen*/
  printf("-------Menu-------\n");
  if (*pId == 0) {
    printf("1) Identify yourself\n");
    printf("2) How to vote and what to vote for!\n");
  } else {
    printf("1) CPR validated: %s\n", CPR);
    printf("2) How to vote and what to vote for!\n");
    printf("3) Vote now!\n");
  }

  /*getting input*/
  scanf("%d", &choice);

  system("CLS");

  /*Finding which choice was asked for*/
  if (choice==1)
  {
    /*----------#1----------*/
    if (*pId == 1) {

      printf("Your identification has been successful, move on to voting!\n");
      sleep(2);
      Menu1(id, CPR);
    } else {
      /* Promt for identification */
      printf("Indtast venligst dit CPR-nummer: ");
      strcpy(CPR, "");
      scanf(" %s", CPR);

      if(verifyIdentity(CPR)) {
        if (binarySearch(&cp, CPR, &position)) {
          printf("You have already voted and aren't allowed to vote again.\n");
        } else {
          moveFileText(&cp, position, CPR);
          *pId = 1;
        }
      } else {
        printf("Incorrect ID.\n");
      }
    }
    Menu1(id, CPR);
  }
  /*----------#2----------*/
  else if (choice==2)
  {
  MenuPartyList();
  }
  /*----------#3----------*/
  else if (choice==3)
  {
    if (*pId == 1) {
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
    } else {
      printf("Please validate yourself!\n");
      sleep(2);
      Menu1(id, CPR);
    }
  }
}
void MenuPartyList(){
int choice;

printf("-------How to vote?-------\n\n");
printf("Under here is a list of parties you can vote for. Every party has a letter attached, seen to the right of the party title. It's this number, you want to enter, when asked what you wanna vote for. For example, if you intent to vote for Alternativet, simply enter 'aa' when voting.\n\nIf you wanna vote for a specific member of a party you have to, in this list, enter the number attached to the party you are voting for, seen to the left of the party title. For example, if you wanna see the list of candidates in Alternativet, simply enter '9', in this list. Follow the instructions on the party page to learn more.\n\n");
printf("-------List-------\n\n");
printf("1) Social Demokratiet\t\ta\n");
printf("2) Radikale Venstre\t\tb\n");
printf("3) Det Konservative Folkeparti\tc\n");
printf("4) Socialistisk Folkeparti\tf\n");
printf("5) Liberal Alliance\t\tl\n");
printf("6) Dansk Folkeparti\t\to\n");
printf("7) Venstre\t\t\tv\n");
printf("8) Enhedslisten\n");
printf("9) Alternativet\n");
printf("10) Nye Borgerlige\n");
printf("11) Kristendemokraterne\n");

/*getting input*/
scanf("%d", &choice);

system("CLS");

/*Finding which choice was asked for (my style of using brackets may be different than yours*/
if (choice==1)
{
SocialDemokratiet();
}
else if (choice==2)
{

}
else if (choice==3)
{

}
else if (choice==4)
{

}
else if (choice==5)
{

}
else if (choice==6)
{

}
else if (choice==7)
{

}
else if (choice==8)
{

}
else if (choice==9)
{

}
else if (choice==10)
{

}
else if (choice==11)
{

}

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
int binarySearch(FILE **cp, char *CPR, int *position) {
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

void SocialDemokratiet(){
  system("CLS");
  printf("Mette Frederiksen: 1\n");
  printf("Ane Halsboe-Joergensen: 2\n");
  printf("Anette Lind: 3\n");
  printf("Astrid Krag: 4\n");
  printf("Benny Engelbrecht: 5\n");
  printf("Bjarne Laustsen: 6\n");
}
