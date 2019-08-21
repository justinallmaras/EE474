/*
Bryan Bednarski, Justin Allmaras, Colin Summers
motor_driver.c
*/

#include "motor_driver.h"
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>



#define BASE_FREQUENCY 2000
#define MAX_BUF 1024





//#define DEBUG
#ifdef DEBUG
// debug variant

#define print(fmt, args...) {\
	char _macro_fmt[100]; \
	snprintf(_macro_fmt, 100, fmt, ##args);\
	printf("%s | %s %s:%d\n", _macro_fmt, __func__, __FILE__, __LINE__);\
}

#else
// prod variant

#define print(fmt, args...)

#endif

#define JOYSTICK_HIGH 1800
#define JOYSTICK_BUF 300
#define JOYSTICK_MID 900

int main() {

    frontDrive = 0;
    
    // define file pointers for two PWM pins, period and duty cycles of output
    print("Setting up PWM pins");
    // creates period and duty cycle folders for PWM pins
    sysPWM = fopen("/sys/devices/bone_capemgr.9/slots" , "w");
    fseek(sysPWM , 0 , SEEK_SET);
    fprintf(sysPWM , "%s" , "am33xx_pwm");
    fflush(sysPWM);
    fseek(sysPWM, 0 , SEEK_SET);

    

    fprintf(sysPWM , "%s" , "bone_pwm_P9_14");
    fflush(sysPWM);
    print("Wait for directories");
    // Waits for PWM test directory to be created
    while(fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/duty" , "w") == NULL);
    fprintf(sysPWM , "%s" , "bone_pwm_P9_16");
    fflush(sysPWM);
    dutyPWM[0] = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/duty" , "w");
    fseek(dutyPWM[0] , 0 , SEEK_SET);
    periodPWM[0] = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/period" , "w");
    fseek(periodPWM[0] , 0 , SEEK_SET);

    while(fopen("/sys/devices/ocp.3/pwm_test_P9_16.16/duty" , "w") == NULL);
    print("Open PWM files");
    // once PWM test directory is created, opens duty-cycle and period
    // folders for PWM pin
    dutyPWM[1] = fopen("/sys/devices/ocp.3/pwm_test_P9_16.16/duty" , "w");
    fseek(dutyPWM[1] , 0 , SEEK_SET);
    periodPWM[1] = fopen("/sys/devices/ocp.3/pwm_test_P9_16.16/period" , "w");
    fseek(periodPWM[1] , 0 , SEEK_SET);
    
    


    print("Initialize GPIO pins");
    // intialize gpio pins and set standby to 0 to avoid any spurious turn on
    standby = initGPIO(standbyPin, OUT, 0);
    usleep(1000000);
    AIN1 = initGPIO(AIN1Pin, OUT, 0);
    AIN2 = initGPIO(AIN2Pin, OUT, 0);
    // FILE* PWMA = initGPIO(AIN1Pin, OUT, 0);
    BIN1 = initGPIO(BIN1Pin, OUT, 0);
    BIN2 = initGPIO(BIN2Pin, OUT, 0);
    // FILE* PWMB = initGPIO(AIN1Pin, OUT, 0);
    // enable the motor driver
    
    FILE* f_x;
    while((f_x = fopen("/sys/devices/ocp.3/helper.17/AIN4", "r")) == NULL);
    FILE* f_y = fopen("/sys/devices/ocp.3/helper.17/AIN5", "r");
    usleep(1000);
    print("Enable motors");
    setGPIOVal(standby , 1);

    // Four interrupt signals for the four sensors
    if (signal(SIG_USER_F, sigHandlerF) == SIG_ERR) {
        print("can't catch SIGINTF");
    }
    if (signal(SIG_USER_B, sigHandlerB) == SIG_ERR) {
        print("can't catch SIGINTB");
    }
    if (signal(SIG_USER_L, sigHandlerL) == SIG_ERR) {
        print("can't catch SIGINTL");
    }
    if (signal(SIG_USER_R, sigHandlerR) == SIG_ERR) {
        print("can't catch SIGINTR");
    }
    // read joystick
    int x = 0;
    int y = 0;
    
    
    while(1) {
        // Read the analog pins for the Joystick
        f_x = fopen("/sys/devices/ocp.3/helper.17/AIN4", "r");
        f_y = fopen("/sys/devices/ocp.3/helper.17/AIN5", "r");
        
        fseek(f_x, 0, SEEK_SET);
        fseek(f_y, 0, SEEK_SET);
        
        fscanf(f_x, "%d", &x);  
        fscanf(f_y, "%d", &y);
        
        fclose(f_x);
        fclose(f_y);

        printf("x: %d   y: %d\n", x, y);
	    if (abs(y-900) > 300 || abs(x - 900) > 300) {
	        if (abs(y - 900) > abs(x - 900)) {
                // move forward or backward
            	if (y > 900) {
                    if(!checkSensor(FRONT)) { 
                	    //move forward
                        printf("forward\n");
                        forward(100);
                    } else {
                        stop();
                    }
            	}                   
            	else {
                	//move backward
                    if(!checkSensor(BACK)) {
                    backward(100);
                    } else {
                        stop();
                    }
            	}
            } else {
                // move left or right
                if (x > 900) {
                    // turn right
                    if(!checkSensor(RIGHT)) {
                        turn_right(100);
                    } else {
                        stop();
                    }
                }
                else {
                    //turn left
		    if(!checkSensor(LEFT)) {
                        turn_left(100);
                    } else {
                        stop();
                    }
                
               }
	    }
        } else {
           stop();
	}	

        usleep(100000);
    }


    // closes all folders for PWM pin
    fclose(sysPWM);
    fclose(periodPWM[0]);
    fclose(dutyPWM[0]);
    fclose(periodPWM[1]);
    fclose(dutyPWM[1]);
    // close_gpio_pins();    
    
    close(front_fd);
    // close pipes
    unlink(front_fifo);
    
    return 0;
}

// Turn the rover right in place with pwm speed
void turn_right(int pwm) {
   setA(0 , 1 , pwm);
   setB(1 , 0 , pwm);
}

// Turn the rover left in place with pwm speed
void turn_left(int pwm) {
   setA(1 , 0 , pwm);
   setB(0 , 1 , pwm);
}

// Move the rover forward with pwm speed
void forward(int pwm) {
   setA(1 , 0 , pwm);
   setB(1 , 0 , pwm);
   
}

// Move the rover backward with pwm speed
void backward(int pwm) {
   setA(0 , 1 , pwm);
   setB(0 , 1 , pwm);
}

// set one track
void setA(int a1 , int a2 , int pwm) {
   setGPIOVal(AIN1 , a1);
   setGPIOVal(AIN2 , a2);
   setPWMVal(dutyPWM[0] , pwm);
}

// set one track
void setB(int b1 , int b2 , int pwm) {
   setGPIOVal(BIN1 , b1);
   setGPIOVal(BIN2 , b2);
   setPWMVal(dutyPWM[1] , pwm);
}

// stop the rover
void stop() {
   setB(0 , 0 , 0);
   setA(0 , 0 , 0);
   print("stop");
}

// Set a pwm duty from 0% to 100%
void setPWMVal(FILE * duty , int dutyCycle) {
    dutyCycle = (100 - dutyCycle) * 5000;
    fprintf(duty , "%d", dutyCycle);
    fflush(duty);
}


// Functions to handle the IR Sensor input
void sigHandlerF(int sigNoFront) {
    state ^= 1 << FRONT;
}

void sigHandlerB(int sigNoBack) {
    state ^= 1 << BACK;
}

void sigHandlerL(int sigNoLeft) {
    state ^= 1 << LEFT;
}

void sigHandlerR(int sigNoRight) {
    state ^= 1 << RIGHT;
}

int checkSensor(int sensorNo) {
    return (state & (1 << sensorNo));
}



