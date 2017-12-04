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

<<<<<<< HEAD
=======
// Hej
>>>>>>> parent of 332d3d0... Mike
/* Main Function */
int main(void) {

  char CPR[CPR_LEN];

  printf("Indtast venligst dit CPR-nummer: ");
  scanf("%[0-9]", CPR);

  printf("%d\n", verifyIdentity(CPR));

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
      if (atoi(CPR_Splittet[0]) > 29) {
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
