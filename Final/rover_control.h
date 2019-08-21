/*
Bryan Bednarski, Justin Allmaras, Colin Summers
rover_control.c
*/

#ifndef ROVER_CONTROL_H
#define ROVER_CONTROL_H

#include "gpio.h"
//#include "timer_interrupts.h"
//#include "pin_interrupts.h"
//#include "LCD.h"  // need
#include "bluetooth.h"
#include "mp3.h"
#include "irSensors.h" //close
#include "nunchuck.h" //close
#include "motor.h" //started
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#define NEW_CHAR_DIR "/dev/new_char"



typedef enum {
   BLUETOOTH,
   OPENCV,
   NUNCHUCK,
   IDLE
} CONTROL_MODE;

CONTROL_MODE MODE;
CONTROL_MODE PREVIOUS_MODE;

// timer interruput structs
struct sigaction sa;
struct itimerval timer;


struct sigaction sa_rover;
struct itimerval timer_rover;
int nunchuckFile;
int left;
int right;

int motorWriteValues[2];

// function prototypes
void setMode();
void runRoverTimerInit();
void runRover(int whatever);
void calculateSpeeds(int x , int y , int * left , int * right);
void driveMotors(int left , int right);
void setIRSensorState();


#endif
