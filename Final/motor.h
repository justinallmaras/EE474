#ifndef MOTOR_H
#define MOTOR_H

    #define DEBUG

    #ifdef DEBUG
    // debug variant

    #define print(fmt, args...) {\
       char _macro_fmt[100]; \
       snprintf(_macro_fmt, 100, fmt, ##args);\
       printf("%s | %s %s:%d\n", _macro_fmt, __func__, __FILE__, __LINE__);\
    }
    #else
    #define print(fmt, args...)
    #endif

    #define MOTOR_DEADBAND 25
    #define MOTOR_MIDPOINT 100
    // pin definitions
    #define AIN1Pin         47
    #define AIN2Pin         27
    #define BIN1Pin         66
    #define BIN2Pin         69
    #define standbyPin      45
    
    #include <termios.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <signal.h>
    #include "gpio.h"
    
    // function prototypes
    char * concat(const char *s1, const char *s2 , const char * s3);
    void writeMotors(unsigned char * driveData , int sensors);
    int motorfd;
    void pwm_conversion(int nJoyX, int nJoyY, int * mixLeftPtr, int * mixRightPtr);
    int checkSensor(int sensor , int s);

#endif