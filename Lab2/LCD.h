// Pin 49: RS   Pin 115: R/W  Pin 112: E

#ifndef LCD_H
#define LCD_H

#include <stdio.h>

   // File pointer declarations for LCD.cgedfi
   FILE *sys[3];
   FILE *dir[3];
   FILE *val[3];
   FILE *dataVal[8];
   FILE *dataDir[8];
   FILE *dataSys[8];
   FILE * RSFile;
   FILE * EFile;
   FILE * RWFile;


   int gpio[3];
   int dataPins[8];
  
   // function prototypes for LCD.c
   void initLCD();
   void initPins();
   void clockStrobe();
   void sendData(int data , int readNWrite , int regSelect);
   void sendChar(char c);
   void cursorShiftLeft(int spaces);
   void cursorShiftRight(int spaces);
   void printLCD(const char * top , const char * bottom);
   void setCursorTopRow();
   void setCursorBottomRow();
   void clearDisplay();
   // void fileClose();
#endif

