
#include "gpio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Takes in a beaglebone pin number , a direction and an inital value
// if applicable and returns a file pointer to the value file of the 
// gpio
FILE * initGPIO (int gpioNumber , int direction , int initialValue) {
   FILE * SYS = fopen("/sys/class/gpio/export" , "w");
   fseek(SYS , 0 , SEEK_SET);
   fprintf(SYS , "%d" , gpioNumber);
   fflush(SYS);
   
   char * gpioNumberStr = malloc(32);
   snprintf(gpioNumberStr , 32 , "%d" , gpioNumber);
   char * dirFile = concat("/sys/class/gpio/gpio" , gpioNumberStr , "/direction");
   FILE * DIR = fopen(dirFile, "w");
   fseek(DIR , 0 , SEEK_SET);
   if (direction == IN) {
      fprintf(DIR , "%s" , "in");
   } else if (direction == OUT) {
      fprintf(DIR , "%s" , "out");
   } else {
      printf("Invalid Direction");
      return NULL;
   }
   fflush(DIR);

   FILE * VAL = NULL;

   char * valFile = concat("/sys/class/gpio/gpio" , gpioNumberStr , "/value");
   if(direction == IN) {
      VAL = fopen(valFile , "r");
   } else if (direction == OUT) {
      VAL = fopen(valFile , "w");
      fseek(VAL , 0 , SEEK_SET);
      fprintf(VAL , "%d" , initialValue);
      fflush(VAL);
   }

   return VAL;
}

// Concatenates three strings
char* concat(const char *s1, const char *s2 , const char * s3)
{
   const size_t len1 = strlen(s1);
   const size_t len2 = strlen(s2);
   const size_t len3 = strlen(s3);
   char *result = malloc(len1 + len2 + len3 + 1); // +1 for the null-terminator
   memcpy(result , s1 , len1);
   memcpy(result + len1 , s2 , len2); // +1 to copy the null-terminator
   memcpy(result + len1 + len2 , s3 , len3 + 1);
   return result;
}

// Function to set the value of a gpio to a 1 or 0 (high or low voltage)
void setGPIOVal(FILE *valFile, int val) {
   fprintf(valFile, "%d", val);
   fflush(valFile);
}

int readGPIOVal(FILE *valFile) {
   int value;
   fscanf(valFile , "%d" , &value);
}

