/*
Bryan Bednarski, Justin Allmaras, Colin Summers
motor_driver.h
*/



#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gpio.h"



#define SIG_USER_F 20
#define SIG_USER_B 21
#define SIG_USER_L 22
#define SIG_USER_R 23


    typedef enum{
        FRONT,
        BACK,
        LEFT,
        RIGHT
    } SENSOR;


    // GPIO Pins used
    int AIN1Pin = 68;
    int AIN2Pin = 45;
    int BIN1Pin = 44;
    int BIN2Pin = 26;
    int standbyPin = 47;
    int PWMAPin = 14;
    int PWMBPin = 16;
    int pwm;
    int frontDrive;
    int backDrive = 0;
    int state = 0;
    FILE * AIN1;
    FILE * AIN2;
    FILE * BIN1;
    FILE * BIN2;
    FILE * sysPWM;
    FILE * dutyPWM[2];
    FILE * periodPWM[2];
    FILE * standby;
    
    // front_fifo variable declarations
    int front_fd;
    char * front_fifo;
    //char buf1;

    // function prototypes
    char * concat(const char *s1, const char *s2 , const char * s3);
    void turn_right(int pwm);
    void turn_left(int pwm);
    void forward(int pwm);
    void backward(int pwm);
    void setA(int a1, int a2, int pwm);
    void setB(int b1, int b2, int pwm);
    void stop();
    void setPWMVal();
    void sigHandlerF(int sigNoFront);
    void sigHandlerB(int sigNoBack);
    void sigHandlerL(int sigNoLeft);
    void sigHandlerR(int sigNoRight);
    int checkSensor(int sensorNo);



#endif
