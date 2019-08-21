


#ifndef MP3_H
#define MP3_H

#define DEBUG
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <string.h>
#include <stdint.h>
#include "gpio.h"
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>

//VS10xx SCI Registers
#define SCI_MODE 0x00
#define SCI_STATUS 0x01
#define SCI_BASS 0x02
#define SCI_CLOCKF 0x03
#define SCI_DECODE_TIME 0x04
#define SCI_AUDATA 0x05
#define SCI_WRAM 0x06
#define SCI_WRAMADDR 0x07
#define SCI_HDAT0 0x08
#define SCI_HDAT1 0x09
#define SCI_AIADDR 0x0A
#define SCI_VOL 0x0B
#define SCI_AICTRL0 0x0C
#define SCI_AICTRL1 0x0D
#define SCI_AICTRL2 0x0E
#define SCI_AICTRL3 0x0F

//global variables
int mp3fd;
int mp3Datafd;
FILE * commandCS;
FILE * dreqfp;
unsigned char *p;
long j;
long lSize;

struct sigaction sa_mp3;
struct itimerval timer_mp3;

//function prototypes
void setupMP3();
int checkDREQ();
void mp3WriteReg (unsigned char addressByte , unsigned char highByte , unsigned char lowByte);
int playTrack();
void pushData(int whatever);

#endif