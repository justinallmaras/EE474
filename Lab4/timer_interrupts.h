/*
Bryan Bednarski, Justin Allmaras, Colin Summers
timer_interrupts.h
*/
#ifndef TIMER_INTERRUPTS_H
#define TIMER_INTERRUPTS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIG_USER_F 20
#define SIG_USER_B 21
#define SIG_USER_L 22
#define SIG_USER_R 23


    // front_fifo variable declarations
    int front_fd;
    char * front_fifo;
    // char buf1;

    // function prototypes
    void front_timer_handler(int whatever);
    //int main(int argc, char *argv[]);
    //void timer_Init();
    //int front_send();

#endif
