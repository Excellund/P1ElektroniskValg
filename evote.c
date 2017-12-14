
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
#define DECRYPT_YEAR 2017
#define DECRYPT_MONTH 12
#define DECRYPT_DAY 14
#define DECRYPT_HOUR 18

/* Prototypes */
int verifyIdentity(char *);
int isLeapYear(char *, char);
int BinarySearch(FILE **, char *, int *);
void moveFileText(FILE **cp, int position, char *CPR);
void stringToHex(char data[DATA_LEN], char data_hex[DATA_LEN * 2]);
int hexToInt(char);
int hexToAscii(char, char);
void hexToString(char data[DATA_LEN], char data_hex[DATA_LEN * 2]);
char *generateKey(char *);
void encrypt(FILE **, char key[DATA_LEN*2], char data_hex[DATA_LEN*2]);
void decrypt(FILE *, FILE *, char key[DATA_LEN*2]);

/* Main Function */
int main(void) {
  char c, CPR[CPR_LEN], person[2], party[2], key[DATA_LEN*2], data[DATA_LEN+1], data_hex[DATA_LEN*2], data_enc[DATA_LEN*2], data_dec[DATA_LEN*2], temp_string[2], data_asc[DATA_LEN];
  unsigned int ch1, ch2;
  int i = 0, position = 0, length, j, buf, size, k;
  time_t t;
  struct tm tm;

  FILE *cp = fopen("identification", "r+");
  FILE *dp = fopen("data_encrypted", "r+");
  FILE *dp_decrypt = fopen("data_decrypted", "r+");

  t = time(NULL);

  srand(time(NULL));

  generateKey(key);

  while (i < 3) {
    tm = *localtime(&t);
    if (tm.tm_year + 1900 >= DECRYPT_YEAR  &&
        tm.tm_mon  + 1    >= DECRYPT_MONTH &&
        tm.tm_mday        >= DECRYPT_DAY   &&
        tm.tm_hour        >= DECRYPT_HOUR) {

      break;
    } else {

      printf("Indtast venligst dit CPR-nummer: ");
      strcpy(CPR, "");
      scanf(" %[0-9]", CPR);

      if (!verifyIdentity(CPR)) {
        printf("Incorrect ID.\n");
      } else {
        if (i > 0 ? BinarySearch(&cp, CPR, &position) : 0) {
          printf("You have already voted and aren't allowed to vote again.\n");
        } else {
          moveFileText(&cp, position, CPR);

          printf("Angiv venligst din stemme for parti og person: ");
          scanf(" %[A-Z]", party);
          scanf(" %[A-Z]", person);

          strcpy(data, "");
          /*strcpy(data, CPR);
          strcat(data, ", ");*/
          strcat(data, party);
          strcat(data, ", ");
          strcat(data, person);

          stringToHex(data, data_hex);

          encrypt(&dp, key, data_hex);
        }
      }
    }
    i++;
  }
  fseek(dp, 0, SEEK_END);
  size = ftell(dp);

  fseek(dp, 0, SEEK_SET);
  for (k = 0; k < size; k += DATA_LEN*2) {
    for (i = 0; i < DATA_LEN*2; i++) {
      c = getc(dp);
      data_enc[i] = c;
    }

    for (i = 0; i < DATA_LEN*2; i += 2) {
      strncpy(temp_string, data_enc + i, 2);
      sscanf(temp_string, "%02X", &ch1);
      strncpy(temp_string, key + i, 2);
      sscanf(temp_string, "%02X", &ch2);
      sprintf(data_dec, "%02X", ch1^ch2);

      length = strlen(data_dec);
      for(j = 0; j < length; j++){
        if(j % 2 != 0){
          data_asc[0] = (char)hexToAscii(buf, data_dec[j]);
        }else{
          buf = data_dec[j];
        }
      }

      fprintf(dp_decrypt, "%c", data_asc[0]);
    }
    fprintf(dp_decrypt, "%c", '\n');
  }

  fclose(cp);
  fclose(dp);
  fclose(dp_decrypt);
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

char *generateKey(char *key) {
  const char charset[] = "0123456789ABCDEF";
  int i, charset_index;

  for (i = 0; i < DATA_LEN * 2; i++) {
    charset_index = rand() % (int) (sizeof charset - 1);
    key[i] = charset[charset_index];
  }
  key[DATA_LEN * 2] = '\0';

  return key;
}

void stringToHex(char data[DATA_LEN], char data_hex[DATA_LEN * 2]) {
  int i;
  for(i = 0; i < DATA_LEN; i++){
    sprintf((data_hex + i * 2), "%02X", data[i]);
  }
  data_hex[DATA_LEN * 2] = '\0';
}

int hexToInt(char c){
        int first = c / 16 - 3;
        int second = c % 16;
        int result = first * 10 + second;
        if(result > 9) {
          result--;
        }
        return result;
}

int hexToAscii(char a, char b){
  int high = hexToInt(a) * 16;
  int low = hexToInt(b);
  return high + low;
}

void hexToString(char data[DATA_LEN], char data_hex[DATA_LEN * 2]) {
  int length = strlen(data_hex);
  int i;
  char buf = 0;
  for(i = 0; i < length; i++){
    if(i % 2 != 0){
      data[i / 2] = hexToAscii(buf, data_hex[i]);
    }else{
      buf = data_hex[i];
    }
  }
}

void encrypt(FILE **dp, char key[DATA_LEN*2], char data_hex[DATA_LEN*2]) {
  char temp_string[2];
  unsigned int ch1, ch2;
  int i;

  for (i = 0; i < DATA_LEN*2; i += 2) {
    strncpy(temp_string, data_hex + i, 2);
    sscanf(temp_string, "%02X", &ch1);
    strncpy(temp_string, key + i, 2);
    sscanf(temp_string, "%02X", &ch2);

    fprintf(*dp, "%02X", ch1^ch2);
  }
}
