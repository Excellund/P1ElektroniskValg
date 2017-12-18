/* This c-file executes a program, which can manage data given as a text file
   and encrypt it with a key symmetrically. At a given time it will then de-
   crypt it using the same key. */
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
#define NEWLINE_SIZE 2
#define HEX_SIZE 2
#define HEX_BASE 16
#define DECRYPT_YEAR 2017
#define DECRYPT_MONTH 12
#define DECRYPT_DAY 18
#define DECRYPT_HOUR 13
#define DECRYPT_MINUTE 53
#define EXIT_SUCCES 0

/* Prototypes */
void stringToHex(char data[DATA_LEN], char data_hex[DATA_LEN * HEX_SIZE]);
int hexToInt(char);
int hexToAscii(char, char);
void hexToString(char data[DATA_LEN], char data_hex[DATA_LEN * HEX_SIZE]);
char *generateKey(char *);
void encrypt(FILE **, char key[DATA_LEN * HEX_SIZE], char data_hex[DATA_LEN * HEX_SIZE]);
void decrypt(FILE *, FILE *, char key[DATA_LEN * HEX_SIZE]);

/* Main Function */
int main(void) {
  char c, key[DATA_LEN * HEX_SIZE], data[DATA_LEN + 1], data_hex[DATA_LEN * HEX_SIZE], data_enc[DATA_LEN * HEX_SIZE], data_dec[DATA_LEN * HEX_SIZE], temp_string[2], data_asc[DATA_LEN + 1];
  unsigned int ch1, ch2;
  int i = 0, length, j, buf, size, k, current, count = 0;
  time_t t;
  struct tm tm;

  /* Initialize file pointers in read and write mode */
  FILE *rp = fopen("results", "r+");
  FILE *dp = fopen("data_encrypted", "r+");
  FILE *dp_decrypt = fopen("data_decrypted", "r+");

  /* Prepare use of time */
  t = time(NULL);
  srand(time(NULL));

  /* Generate key for encryption */
  generateKey(key);

  /* Run until the voting phase has finished */
  while (TRUE) {

    /* Get the current time */
    t = time(NULL);
    tm = *localtime(&t);

    /* if the voting phase is over */
    if (tm.tm_year + 1900 >= DECRYPT_YEAR  &&
        tm.tm_mon  + 1    >= DECRYPT_MONTH &&
        tm.tm_mday        >= DECRYPT_DAY   &&
        tm.tm_hour        >= DECRYPT_HOUR  &&
        tm.tm_min         >= DECRYPT_MINUTE) {

      /* Get amount of encrypted data */
      fseek(dp, 0, SEEK_END);
      size = ftell(dp);


      fseek(dp, 0, SEEK_SET);

      /* For all data in file */
      for (k = 0; k < size; k += DATA_LEN * HEX_SIZE) {

        /* Get data from a single vote */
        for (i = 0; i < DATA_LEN * HEX_SIZE; i++) {
          c = getc(dp);
          data_enc[i] = c;
        }

        /* XOR cipherfile and key to get decrypted data as hexadecimal */
        for (i = 0; i < DATA_LEN * HEX_SIZE; i += HEX_SIZE) {
          strncpy(temp_string, data_enc + i, HEX_SIZE);
          sscanf(temp_string, "%02X", &ch1);
          strncpy(temp_string, key + i, HEX_SIZE);
          sscanf(temp_string, "%02X", &ch2);
          sprintf(data_dec, "%02X", ch1^ch2);

          /* Convert to ASCII */
          length = strlen(data_dec);
          for(j = 0; j < length; j++){
            if(j % HEX_SIZE != 0){
              data_asc[0] = (char)hexToAscii(buf, data_dec[j]);
            }else{
              buf = data_dec[j];
            }
          }

          /* Write data to file */
          fprintf(dp_decrypt, "%c", data_asc[0]);
        }
        fprintf(dp_decrypt, "%c", '\n');
      }
      printf("Decrypted.\n");

      /* Close files */
      fclose(dp_decrypt);
      fclose(dp);
      fclose(rp);
      break;

    /* If it isn't time to decrypt */
    } else {

      /* Position pointer to file */
      fseek(rp, count * (DATA_LEN + NEWLINE_SIZE), SEEK_SET);
      fseek(dp, count * (DATA_LEN * HEX_SIZE), SEEK_SET);
      current = ftell(rp);
      fseek(rp, 0, SEEK_END);

      /* If there is data left in the file */
      if (!(ftell(rp) - current < 1)) {

        /* Scan data from results file created by vote.c */
        fseek(rp, current, SEEK_SET);
        strcpy(data, "");
        fscanf(rp, "%[^\n]\n", data);

        /* Convert to hexadecimal */
        stringToHex(data, data_hex);

        /* Encrypt the data */
        encrypt(&dp, key, data_hex);

        /* Move down a line in the files */
        count++;
      }
    }
  }

  /* When done decrypting: Close files */
  fclose(dp);
  fclose(rp);

  return EXIT_SUCCES;
}

/* Generates a key in hexadecimal form */
char *generateKey(char *key) {
  const char charset[] = "0123456789ABCDEF";
  int i, charset_index;

  for (i = 0; i < DATA_LEN * HEX_SIZE; i++) {
    charset_index = rand() % (int) (sizeof charset - 1);
    key[i] = charset[charset_index];
  }
  key[DATA_LEN * HEX_SIZE] = '\0';

  return key;
}

/* Converts a char array of ASCII to char array of hexadecimal */
void stringToHex(char data[DATA_LEN], char data_hex[DATA_LEN * HEX_SIZE]) {
  int i;
  for(i = 0; i < DATA_LEN; i++){
    sprintf((data_hex + i * HEX_SIZE), "%02X", data[i]);
  }
  data_hex[DATA_LEN * HEX_SIZE] = '\0';
}

int hexToInt(char c){
  int first = c / HEX_BASE - 3;
  int second = c % HEX_BASE;
  int result = first * 10 + second;
  if(result > 9) {
    result--;
  }
  return result;
}

int hexToAscii(char a, char b){
  int high = hexToInt(a) * HEX_BASE;
  int low = hexToInt(b);
  return high + low;
}

/* Converts a char array of hexadecimal to char array of ASCII */
void hexToString(char data[DATA_LEN], char data_hex[DATA_LEN * HEX_SIZE]) {
  int length = strlen(data_hex);
  int i;
  char buf = 0;
  for(i = 0; i < length; i++){
    if(i % HEX_SIZE != 0){
      data[i / HEX_SIZE] = hexToAscii(buf, data_hex[i]);
    }else{
      buf = data_hex[i];
    }
  }
}

/* Encrypt data using a key, and print the outpput to a textfile */
void encrypt(FILE **dp, char key[DATA_LEN * HEX_SIZE], char data_hex[DATA_LEN * HEX_SIZE]) {
  char temp_string[HEX_SIZE];
  unsigned int ch1, ch2;
  int i;

  for (i = 0; i < DATA_LEN * HEX_SIZE; i += HEX_SIZE) {
    strncpy(temp_string, data_hex + i, HEX_SIZE);
    sscanf(temp_string, "%02X", &ch1);
    strncpy(temp_string, key + i, HEX_SIZE);
    sscanf(temp_string, "%02X", &ch2);
    fprintf(*dp, "%02X", ch1^ch2);
  }
}
