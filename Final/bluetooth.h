
#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <poll.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>

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

int bluetooth_fd;
int bluetoothIsDirty;

struct sigaction sa_bluetooth;
struct itimerval timer_bluetooth;

struct pollfd commands[1];
char buf[16];
// x , y , but 1 , but 2 , but 3 , but 4 , but 5 , but 6
int bluetoothData[8];
char rawData[16];

typedef struct {
   int fd;
   int events;
   int revents;   
} pollfd;

// function prototypes
void setupBluetooth();
int getBluetoothRawData();
void getBluetoothData (int whatever);


#endif