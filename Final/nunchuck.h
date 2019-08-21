/*
Bryan Bednarski, Justin Allmaras, Colin Summers
nunchuck.h
*/


#ifndef NUNCHUCK_H
#define NUNCHUCK_H

#ifndef PORT
#define PORT "/dev/i2c-1"    // reserved for capes and requiring pullup resistors
#endif			     // p9:pin19=SCL, p9:pin20:SDA

#ifndef ADDR
#define ADDR  0x52            // wii nunchuck address: 0x52
#endif

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>


#define error(x...) { fprintf(stderr, "\nE%d: ", __LINE__); fprintf(stderr, x); fprintf(stderr, "\n\n"); }

struct sigaction sa_nunchuck;
struct itimerval timer_nunchuck;
unsigned char nunchuckBuf[6];
int fd;
int command;

// function prototypes
int  setupNunchuck();
void get_nunchuck_data(int file);

#endif
