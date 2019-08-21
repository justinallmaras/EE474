/*
EE474 Fall 2016
Justin Allmaras, Bryan Bednarski, Colin Summers
count.c

This file, when compiled and executed in C controls both the binary seven-count LED
and breadboard speaker drivers. On the tank, three LEDs count from zero to seven, displaying
their current count in a binary sequence. In addition, a speaker projects corresponding 
notes on the chromatic scale (multiples of 440). Enjoy!

*/

// Includes standard, time and math library imports.
#include <stdio.h>
#include <time.h>
#include <math.h>

// Base frequency for speaker driver is a mutliple of A - 440Hz
#define BASE_FREQUENCY 440

// Driver for LEDS and speaker included in main()
int main() {

  // Define files pointers for 3 LED drivers system, direction and values
  FILE *sys[3];
  FILE *dir[3];
  FILE *val[3];

  // Defines file pointers for one PWM pin, period and duty cycle of output
  FILE *sysPWM;
  FILE *periodPWM;
  FILE *dutyPWM;

  // Selects three specific GPIO pins on beaglebone
  int i;
  int gpio[3];
  gpio[0] = 49;
  gpio[1] = 112;
  gpio[2] = 115;

  // Creates dir and val folders
  for(i = 0; i < 3; i++) {
    sys[i] = fopen("/sys/class/gpio/export" , "w");
    fseek(sys[i] , 0 , SEEK_SET);
    fprintf(sys[i] , "%d" , gpio[i]);
    fflush(sys[i]);
  }

  // creates period and duty cycle folders for PWM pin
  sysPWM = fopen("/sys/devices/bone_capemgr.9/slots" , "w");
  fseek(sysPWM , 0 , SEEK_SET);
  fprintf(sysPWM , "%s" , "am33xx_pwm\n");
  fflush(sysPWM);
  fseek(sysPWM , 0 , SEEK_SET);
  fprintf(sysPWM , "%s" , "bone_pwm_P9_14\n");
  fflush(sysPWM);

  // opens all direction fodlers for GPIO pins
  dir[0] = fopen("/sys/class/gpio/gpio49/direction" , "w");
  dir[1] = fopen("/sys/class/gpio/gpio112/direction" , "w");
  dir[2] = fopen("/sys/class/gpio/gpio115/direction" , "w");

  // Writes to each direction folder of GPIO and sets to output
  for(i = 0; i < 3; i++) {
    fseek(dir[i] , 0 , SEEK_SET);
    fprintf(dir[i] , "%s" , "out");
    fflush(dir[i]);
  }

  // opens all value folders for GPIO pins
  val[0] = fopen("/sys/class/gpio/gpio49/value" , "w");
  val[1] = fopen("/sys/class/gpio/gpio112/value" , "w");
  val[2] = fopen("/sys/class/gpio/gpio115/value" , "w");

  for(i = 0; i < 3; i++) {
    fseek(val[i] , 0 , SEEK_SET);
  }

  // Waits for PWM test directory to be created
  while(fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/duty" , "w") == NULL);

  // once PWM test directory is created, opens duty-cycle and period
  // folders for PWM pin
  dutyPWM = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/duty" , "w");
  fseek(dutyPWM , 0 , SEEK_SET);
  periodPWM = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/period" , "w");
  fseek(periodPWM , 0 , SEEK_SET);

  // variable declarations for main functional loop
  int frequency;
  int count = 0;
  int countVals[3];
  int loopCount = 0;

  // Functional loop of program, counts LEDS, and calculates corresponding periods
  // for the note to play from speaker at each value of the counter.
  while(loopCount < 10) {
    countVals[0] = count % 2;
    countVals[1] = (count / 2) % 2;
    countVals[2] = count / 4;
    for(i = 0; i < 3; i++) {
      fprintf(val[i] , "%d" , countVals[i]);
      fflush(val[i]);
    }
    // Equation for frequencies of chromatic scale
    frequency = (1 / (BASE_FREQUENCY * pow(2.0, count / 12.0))) * pow(10 , 9);
    fprintf(dutyPWM , "%d" , frequency / 2);
    fflush(dutyPWM);
    fprintf(periodPWM , "%d" , frequency);
    fflush(periodPWM);
    usleep(500000);
    count++;
    if (count == 8) {
      count = 0;
      loopCount++;
    }
  }

  // closes all folders for GPIO pins
  for(i = 0; i < 3; i++) {
    fclose(sys[i]);
    fclose(dir[i]);
    fclose(val[i]);
  }

  // closes all folders for PWM pin
  fclose(sysPWM);
  fclose(periodPWM);
  fclose(dutyPWM);

  return 0;
}
