
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
#define DECRYPT_YEAR 2017
#define DECRYPT_MONTH 12
#define DECRYPT_DAY 15
#define DECRYPT_HOUR 11
#define DECRYPT_MINUTE 39
#define EXIT_SUCCES 0

/* Prototypes */
void stringToHex(char data[DATA_LEN], char data_hex[DATA_LEN * 2]);
int hexToInt(char);
int hexToAscii(char, char);
void hexToString(char data[DATA_LEN], char data_hex[DATA_LEN * 2]);
char *generateKey(char *);
void encrypt(FILE **, char key[DATA_LEN*2], char data_hex[DATA_LEN*2]);
void decrypt(FILE *, FILE *, char key[DATA_LEN*2]);

/* Main Function */
int main(void) {
  char c, key[DATA_LEN*2], data[DATA_LEN+1], data_hex[DATA_LEN*2], data_enc[DATA_LEN*2], data_dec[DATA_LEN*2], temp_string[2], data_asc[DATA_LEN+1];
  unsigned int ch1, ch2;
  int i = 0, length, j, buf, size, k, current, count = 0;
  time_t t;
  struct tm tm;

  t = time(NULL);
  srand(time(NULL));

  generateKey(key);

  FILE *rp = fopen("results", "r+");
  FILE *dp = fopen("data_encrypted", "r+");
  FILE *dp_decrypt = fopen("data_decrypted", "r+");

  while (TRUE) {

    t = time(NULL);
    tm = *localtime(&t);
    printf("%d %d\n", tm.tm_min, tm.tm_sec);
    if (tm.tm_year + 1900 >= DECRYPT_YEAR  &&
        tm.tm_mon  + 1    >= DECRYPT_MONTH &&
        tm.tm_mday        >= DECRYPT_DAY   &&
        tm.tm_hour        >= DECRYPT_HOUR &&
        tm.tm_min         >= DECRYPT_MINUTE) {

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
      printf("Decrypted.\n");
      fclose(dp_decrypt);
      fclose(dp);
      fclose(rp);
      break;
    } else {
      fseek(rp, count * (DATA_LEN + 2), SEEK_SET);
      fseek(dp, count * (DATA_LEN * 2), SEEK_SET);
      current = ftell(rp);
      fseek(rp, 0, SEEK_END);
      if (!(ftell(rp) - current < 1)) {
        printf("%d\n", current);
        fseek(rp, current, SEEK_SET);
        strcpy(data, "");
        fscanf(rp, "%[^\n]\n", data);

        printf("%s\n", data);

        stringToHex(data, data_hex);
        encrypt(&dp, key, data_hex);
        count++;
      }
    }
  }

  fclose(dp);
  fclose(rp);

  return EXIT_SUCCES;
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
