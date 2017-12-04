#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Constants */
#define TRUE 1
#define FALSE 0
#define CPR_LEN 10
#define EXIT_SUCCES 0

/* Prototypes */
int verifyIdentity(char *CPR);
void RemoveCharacter(char* source, char character);

/* Main Function */
int main(void) {

  char CPR[CPR_LEN];

  printf("Indtast venligst dit CPR-nummer: ");
  scanf("%[0-9]", CPR);

  verifyIdentity(CPR);

  return EXIT_SUCCES;
}

int verifyIdentity(char *CPR) {

  char CPR_Splittet[4][5];
  int i = 0;

  for (i = 0; i < CPR_LEN - 2 ; i += 2) {
    strncpy(CPR_Splittet[i/2], CPR + i, i == 6 ? 4 : 2);
    CPR_Splittet[i/2][i == 6 ? 4 : 2] = 0;
  }

  /*
    Convert to integers
    Check if right amount of dates
    Remember february
  */

  return TRUE;
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
