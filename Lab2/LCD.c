/*
EE474 Fall 2016
Justin Allmaras, Bryan Bednarski, Colin Summers
LCD.C

This file is the driver for our LCD board and contains methods necessary
to intialize specific GPIO pins, initialize the LCD board, and
abstract a number of LCD board functions for use in higher-level c functions.

*/

#include <stdio.h>
#include <time.h>
#include "LCD.h"
#include "gpio.h"
#include <unistd.h>


// Pin 49: RS   Pin 115: R/W  Pin 112: E
// updates data 0:66 data 1:69 data 2:45 data 3:67 data 4:68 data 5:44  data 6:26 data 7:46
// function declaraions in LCD header file


void initPins() {
   // define pin numbers for 3 GPIO pins
   int RSPin = 49;
   int EPin = 112;
   int RWPin = 115;

   // intializes three GPIO control pins for LCD
   RSFile = initGPIO(RSPin , OUT , 0);
   EFile = initGPIO(EPin , OUT , 0);
   RWFile = initGPIO(RWPin , OUT , 0);
   int i;



   // define pin numbers for 8 data pins
   dataPins[0] = 66;
   dataPins[1] = 69;
   dataPins[2] = 45;
   dataPins[3] = 67;
   dataPins[4] = 68;
   dataPins[5] = 44;
   dataPins[6] = 26;
   dataPins[7] = 46;

   // initializes 8 data pins for LCD data bus
   for(i = 0; i < 8; i++) {
      dataVal[i] = initGPIO(dataPins[i] , OUT , 0);
   }
}

// initialize the LCD display.  Call before writing data to the LCD
void initLCD() {
   initPins();
   // wait more than 15ms after power on before initializing
   usleep(20000); // 20ms
   // send 0011xxxx - function set command
   sendData(0x30 , 0 , 0);
   // wait at least 4.1ms
   usleep(5000);
   // send 0011xxxx
   sendData(0x30 , 0 , 0);
   // wait at least 100 microseconds
   usleep(150);
   // function set with number of lines and font select
   sendData(0x30 , 0 , 0);
   // function set - number of lines (2) and display font (5x10) can be sent
   usleep(40);
   sendData(0x3C , 0 , 0);
   // delays to wait for possible busy flag. Consider using gpio input to check busy flag 
   usleep(40);
   // Display OFF
   sendData(0x08 , 0 , 0);
   // clear display
   sendData(0x01 , 0 , 0);
   usleep(40);
   // entry set mode
   sendData(0x06 , 0 , 0);
   usleep(40);
   // display ON with cursor and blink
   sendData(0x0F , 0 , 0);
   usleep(40);
}

// Clock strobe function necessary for sending characters to the data pins.
// FUnciton is used in sendData() function below
void clockStrobe() {
   fprintf(EFile , "%d" , 1);
   if(fflush(EFile) != 0) {
      perror("did not write 1");
   }
   // wait at least 450 ns
   usleep(1);
   fprintf(EFile , "%d" , 0);
   if(fflush(EFile) != 0){
      perror("did not write 0");
   }
   usleep(1);
}

// this function sends 8 bits of data in parallel across the 8 GPIO
// data pins to the LCD. Strobes clock to write each data bit
void sendData(int data , int writeNRead , int regSelect) {
   fprintf(RSFile , "%d" , regSelect);
   fflush(RSFile);
   fprintf(RWFile , "%d" , writeNRead);
   fflush(RWFile);
   int i;
   int dataEightBit = data % 256;
   for(i = 0; i < 8; i++) {
      fprintf(dataVal[i] , "%d" , dataEightBit % 2);
      fflush(dataVal[i]);
      dataEightBit /= 2;
   }

   usleep(2);
   clockStrobe();
   usleep(40);
}

// Sinds individual character to LCD
void sendChar(char c) {
   sendData(c , 0 , 1);
 
}

// Shifts cursor left by the number of spaces passed to function
void cursorShiftLeft(int spaces) {
   int i;
   for(i = 0; i < spaces; i++) {
      sendData(0x10 , 0 , 0);
   }
}

// Shifts cursor right by the number of spaces passed to the function
void cursorShiftRight(int spaces) {
   int i;
   for(i = 0; i < spaces; i++) {
      sendData(0x14 , 0 , 0);
   }
}

// Clears display for next string to be written
void clearDisplay () {
   sendData(0x01 , 0 , 0);
   usleep(15200);
}

// Moves cursor to bottom row 
void setCursorBottomRow() {
   sendData(0xC0 , 0 , 0);
}

// Moves cursor to top row
void setCursorTopRow() {
   sendData(0x80 , 0 , 0);
}


// Prints current buffered string of chatacters to the LCD screen
// using the sendChar function
void printLCD(const char * top , const char * bottom) {
   //clear display
   //return cursor home
   clearDisplay();
   int i;
   // Prints to top row
   setCursorTopRow();
   for(i = 0; i < 16; i++) {
      if(top[i] != '\0') {
	   sendChar(top[i]);
	} else {
       sendChar(' ');
	}
   }
   // Prints to bottom row
   setCursorBottomRow();   
   for(i = 0; i < 16; i++) {
      if(bottom[i] != '\0') {
	   sendChar(bottom[i]);
	} else {
       sendChar(' ');
	}
   }


}

/* 
// is it neccessary to close all fines after sending a character to them?
void fileClose() {
   int i, j;    
   for(i = 0; i < 3; i++) {
      fclose(sys[i]);#include <unistd.h>
      fclose(dir[i]);
      fclose(dir[i]);
    }

    for(j = 0; j < 8; j++) {
       fclose(dataSys[j]);
       fclose(dataDir[j]);
       fclose(dataVal[j]);
    }
}
*/
