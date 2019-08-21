/*
Bryan Bednarski, Justin Allmaras, Colin Summers
timer_interrupts.c
*/


#include "timer_interrupts.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#define SIG_USER 20
//20 mV dead band in each direction
#define DEADBAND 200


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


struct sigaction sa;
struct itimerval timer;
int pid;
void timer_Init();
int front_send(int val);
int analogBuf;
FILE * analog;
//Front Back Left Right
int sigUser [4] = {20 , 21 , 22 , 23};
char dir[40];

int state = 0;

//timer interrupt handler
void front_timer_handler(int whatever) {
  /*send user defined signal (SIG_USER) to the process with PID
    kill() doesn't neccessarily mean to terminate another process,
    and in this case, it means to send signal "SIG_USER" to the
    process with "pid"
  */
    int i;
    for (i = 0; i < 4; i++) {
        sprintf(dir , "/sys/devices/ocp.3/helper.17/AIN%d" , i);
        analog = fopen(dir , "r");
        fseek(analog , 0 , SEEK_SET);
        fscanf(analog , "%d" , &analogBuf);
        fclose(analog);
        if(((analogBuf > (900 + DEADBAND)) && !(state & (1 << i))) || ((analogBuf <= (900 - DEADBAND)) && (state & (1 << i)))) {
            if(kill(pid , sigUser[i]) != 0){
                printf("Can't send msg\n");
                exit(0);
            }
            printf("Signal sent\n");
            state ^= (1 << i); 
        }
        print("Analog %d: %d" , i , analogBuf);
    }
}

int main(int argc, char *argv[])
{	
  analogBuf = 0;

  FILE * sys;
  sys = fopen("/sys/devices/bone_capemgr.9/slots" , "w");
  fseek(sys , 0 , SEEK_SET);
  fprintf(sys , "%s" , "cape-bone-iio");
  fflush(sys);
  fseek(sys, 0 , SEEK_SET);
  
  while(fopen("/sys/devices/ocp.3/helper.17/AIN0" , "r") == NULL);
  
  int count;

  printf ("This program was called with \"%s\".\n",argv[0]);

  if (argc > 1)
  {
    for (count = 1; count < argc; count++)
    {
      printf("argv[%d] = %s\n", count, argv[count]);
    }
  }else{
    printf("The command needs more arguments.\n"); 
    return -1; 
  }

  pid = atoi(argv[1]);
  timer_Init();
  while(1);

	return 0;
}

void timer_Init()
{
   memset (&sa, 0, sizeof (sa));
   sa.sa_handler = &front_timer_handler;
   sigaction (SIGVTALRM, &sa, NULL);

   // Configure the timer to expire after 100 msec...
   timer.it_value.tv_sec = 0;
   timer.it_value.tv_usec = 100000;
   // ... and every 100 msec after that. 
   timer.it_interval.tv_sec = 0;
   timer.it_interval.tv_usec = 100000;
   // Start a virtual timer. It counts down whenever this process is executing. 
   setitimer (ITIMER_VIRTUAL, &timer, NULL);
}

//int front_send(int val)
//{
//    front_fifo = "/tmp/front_fifo";
//    char * error_Msg = "Error opening file, checking if file exists.\n";
//    int front_val;
//    /* create the FIFO (named pipe) *
//
//    /* write "Hi" to the FIFO */
//    if( (front_fd = open(front_fifo, O_WRONLY)) < 0){
//        //write(2, error_Msg, sizeof(error_Msg));
//        printf("%s", error_Msg);
//        return -1; //return error code 
//    }
//    write(front_fd, &front_val, sizeof(front_val));
//    close(front_fd); 
//
//    /* remove the FIFO */
//    //unlink(front_fifo);
//    return 0;
//}
