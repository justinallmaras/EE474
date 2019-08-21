
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

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



typedef struct {
   int fd;
   int events;
   int revents;   
} pollfd;




int main () {

   
   //char * commandsPipe = "/tmp/LCDPipe";
   char * pipes[2] = {"/tmp/comm" , "/tmp/data"};
   if(mkfifo(pipes[0] , 0666)) {
      print("ERR %d %s" , errno, strerror(errno));
   }
   if(mkfifo(pipes[1] , 0666)) {
      print("ERR %d %s" , errno, strerror(errno));
   }

   struct pollfd commands[2];
   char buf[16];
   memset(buf , '\0' , 16 * sizeof(char));
   //int writer = open(commandsPipe , O_WRONLY | O_NONBLOCK);

   commands[0].fd = open(pipes[0] , O_RDONLY | O_NONBLOCK);
   commands[1].fd = open(pipes[1] , O_RDONLY | O_NONBLOCK);
   commands[0].events = POLLIN;
   commands[1].events = POLLIN;
   commands[0].revents = 0;
   commands[1].revents = 0;
   int done = 0;
   //print("RDBAND: %d , RDNORM: %d , OUT: %d , HUP: %d" , POLLRDBAND , POLLRDNORM , POLLOUT , POLLHUP);
   int i;

   int read_ret = 1;
   while (done == 0) {
      int ret = poll(commands , 2 , 500);
      print("polled");
      if(ret > 0) {
         for (i = 0; i < 2; i++) {
            // if(commands[i].revents & POLLRDBAND) {
            //    print("POLLRDBAND: %d"  , i);
            // }
            if(commands[i].revents & POLLIN) {
               // TODO:  Fix reading to the same buffer
               print("POLLIN: %d"  , i);
               while(1) {
                  read_ret = read(commands[i].fd, buf, 16);
                  while(read_ret > 0) {
                     print("READRET: %d STRING: %.*s" ,read_ret , read_ret, buf);
                     read_ret = read(commands[i].fd, buf, 16);
                  }
                  if (read_ret == -1) {
                     if (errno == EAGAIN) {
                        print("here %d", read_ret);
                        break;
                     } else if (errno == EINTR) {
                        print("here %d", read_ret);
                        read_ret = 1;
                     }
                  } else if (read_ret == 0) {
                     print("here %d", read_ret);
                     close(commands[i].fd);
                     commands[i].fd = open(pipes[i] , O_RDONLY | O_NONBLOCK);
                     break;
                  }
               }

            } if((commands[i].revents & POLLHUP) && (read_ret != 0)) {
               print("POLLHUP: %d"  , i);
               close(commands[i].fd);
               commands[i].fd = open(pipes[i] , O_RDONLY | O_NONBLOCK);
            }
         }
      } else {
         // print("WRBAND %d: %d , OUT: %d , HUP: %d" , i , POLLWRBAND , POLLOUT , POLLHUP);
      }
      usleep(200000);
   }

   close(commands[0].fd);
   remove(pipes[0]);
   close(commands[1].fd);
   remove(pipes[1]);
   
	return 0;
}
