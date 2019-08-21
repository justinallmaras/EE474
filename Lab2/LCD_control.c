#include "LCD.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>

#define MAX_BUF 32
#define ROW_SIZE 16


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


// main function, includes calls to initialization sequence (pins and LCD)
int main() {
   // initialize LCD display
   // must init pins before LCD init is possible
   initLCD();

   // Wait one second after initialization
   usleep(1000000);
   typedef enum {
      SCROLL,
      FREE ,
      UNKNOWN
   } MODE;

   //default to unknown mode on startup
   MODE mode = UNKNOWN;
   char buf[MAX_BUF];
   memset(&buf, 0, MAX_BUF);
   char top[ROW_SIZE];
   char bottom[ROW_SIZE];
   memset(top, 0, ROW_SIZE);
   memset(bottom, 0, ROW_SIZE);

   char command[2];
   char * LCDDataPipe = "/tmp/LCDData";
   char * LCDCommandPipe = "/tmp/LCDCommands";
   mkfifo(LCDCommandPipe , 0666);
   mkfifo(LCDDataPipe, 0666);

   // initialize flags for when to update the LCD screen
   int commandDirty = 0;
   int dataDirty = 0;
   int read_rtn1;
   int read_rtn2;


   // intialize pollfd structs
   struct pollfd my_fds[2];
   my_fds[0].events = POLLIN;
   my_fds[0].revents = 0;
   //my_fds[0].fd = open(LCDCommandPipe , O_RDONLY | O_NONBLOCK); 
   my_fds[1].events = POLLIN;
   my_fds[1].revents = 0;
   // my_fds[1].fd = open(LCDDataPipe , O_RDONLY | O_NONBLOCK);


   // inifinite loop to open reading pipes and check if data is ready
   // if data is on the pipe, this will read the data and print the
   // appropriate message to the screen or change the mode accordingly
   while(1) {
      memset(buf, '\0', MAX_BUF);
      memset(command, '\0', 2);
      my_fds[0].fd = open(LCDCommandPipe , O_RDONLY | O_NONBLOCK);
      my_fds[1].fd = open(LCDDataPipe , O_RDONLY | O_NONBLOCK);
      usleep(500000);
	   if(read_rtn1 = (read(my_fds[0].fd, command, 2)) > 0) {
         commandDirty = 1;
	      print("%.*s", 2 ,command);
	   }
	   if(read_rtn2 = (read(my_fds[1].fd, buf, MAX_BUF)) > 0) {
         dataDirty = 1;
	   print("%.*s", MAX_BUF, buf);
	}
	print("fd[0]: %d   fd[1}:   %d   POLLIN:  %d", my_fds[0].revents, my_fds[1].revents, POLLIN);
	//print("1: %d 2: %d", read_rtn1, read_rtn2);
   close(my_fds[1].fd);
   close(my_fds[0].fd);

   // update the mode of the LCD and clear the display
   if(commandDirty == 1) {
      clearDisplay();
      switch (command[0]) {
      case '#':
	      print("case #");
         mode = SCROLL;
         memset(bottom, 0, sizeof(char) * ROW_SIZE);
         sendData(0x0C , 0 , 0);
         break;
      case '$':
	      print("case $");
         mode = FREE;
         sendData(0x0C , 0 , 0);
         break;
      }
      commandDirty = 0;
   }

   // Print data to the LCD in the appropriate mode
   if(dataDirty == 1) {
      if(buf != NULL) {
   	   clearDisplay();  //check if needed
         switch (mode) {
         case SCROLL:
   	    print("here1");
            memcpy(top , bottom , sizeof(char) * ROW_SIZE);
            memcpy(bottom , buf , sizeof(char) * ROW_SIZE);
            print("here3");
            break;
         case FREE:
   	    //print("str len %d", str_len);
            memcpy(top , buf , sizeof(char) * ROW_SIZE);
            memcpy(bottom , buf + sizeof(char) * ROW_SIZE, sizeof(char) * ROW_SIZE);           
            //memset(&bottom[(str_len - 16) % 17], 'X', sizeof(char) * (16 - (str_len - 16) % 17));
   	    //printf("buf: %.*s\n", 32, buf);                
   	    //printf("top: %.*s\n", 16, top); 
   	    //printf("bot: %.*s\n", 16, bottom);                               
            break;
         default:
         break;               
         }
            printLCD(top , bottom);
            //memset(&buf, 0, MAX_BUF);
            //memset(&top, 0, ROW_SIZE);
            //memset(&bottom, 0, ROW_SIZE);
         }
         dataDirty = 0;
      }
   }
   
   // fileClose();
   return 0;
}
