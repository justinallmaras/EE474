#ifndef IRSENSORS_H
#define IRSENSORS_H

#define IR_SENSOR_TRIGGER_VALUE		500
#define IR_SENSOR_DEADBAND			30

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>

struct sigaction sa_sensor;
struct itimerval timer_Sensor;
unsigned int sensorBuf[4];

void setupIRSensors();
int readADC(int sensor);
void read_all_ADC(int whatever);

int irSensorsIsDirty;
unsigned char irSensorState;

typedef enum {
    FRONT,
    BACK,
    LEFT,
    RIGHT
} SENSOR;

#endif
