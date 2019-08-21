#ifndef GPIO_H
#define GPIO_H
   #include <stdio.h>

   // direction enum for readability
   typedef enum{
      IN,
      OUT
   } DIRECTION;

   // function prototypes for gpio.c
   void setGPIOVal(FILE *valFile, int val);
   FILE * initGPIO (int gpio , int dir , int value);
   char * concat(const char *s1, const char *s2 , const char * s3);

#endif
