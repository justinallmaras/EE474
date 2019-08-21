#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "pipe_write.h"

#define MAX_BUF 17

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




int main() {
   print("main");
   int writer;
   char * line = "HELLOOO";
   char * line1 = "world";
   char command[1];
   
   print("main");



   int writerData;
   int writerCommands;
   char nextLine[16];
   char * LCDData = "/tmp/LCDData";
   char * LCDCommands = "/tmp/LCDCommands";    
   // write to the pipe
   print("here0");
   usleep(1000000);
   writerData = open(LCDData, O_WRONLY | O_NONBLOCK);
   print("here1");
   writerCommands = open(LCDCommands , O_WRONLY | O_NONBLOCK);
   print("here");
   int ret;
   char buf[50000];
   memset(buf, '\0', 50000);
   int BUF_SIZE = 50000;
   // ret = readFile(buf, 50000, "/home/colinxs/Projects/group5/Lab2/star_wars.txt");
   // printf("%s", buf);
   char a[16];
   // centerText("hello world", 10, 16, a);
   // printf("%/s", a);

      print("start");
      command[0] = '#';
      write(writerCommands , command , 1);
   while(1) {
      FILE* fin = fopen("/Lab2/star_wars.txt", "r");
      if (fin == NULL) {
         print("return")
         return 1;
      }
      while ((fgets(buf, BUF_SIZE, fin)) != NULL) {
         print("start");
         command[0] = '#';
         // write(writerCommands , command , 1);
         lineWrap(buf, 16);

         int base = 0;
         for (int i = 0;i < strlen(buf); i++) {
            if (buf[i] == '\n') {
               // char result[16];
               // memset(result, ' ', 16);
               //printf("%.*s\n", i - base, &buf[base]);
               write(writerData ,&buf[base] , i - base);
               base = i + 1;
               // centerText(&buf[base], 16, 16, result);
               // printf("%.16s", result);
               usleep(1000000);
            }
         }


         // printf("HERE: %s\n\n", buf);
         // write(writerData , line1 , MAX_BUF - 1);
         // usleep(500000);

         // printf("%s", result);
         // while(result != NULL) {
         //    result = strtok(NULL, "\n");
         //    printf("%s", result);
         //    printf("\n");
         //    // centerText(result, 16, 16, result);
         //    // printf("%s", result);
         //    usleep(100);
         // }

      }
      fclose(fin); 
      return 0;



      // usleep(1000000);
      // write(writerData , line , 7);
      // usleep(1000000);
      // write(writerData , line1 , MAX_BUF - 1);
      // usleep(1000000);
      // command[0] = '$';
      // write(writerCommands , command , 1);
      // usleep(1000000);
      // write(writerData , line , 7);
      // usleep(1000000);
      // write(writerData , line1 , MAX_BUF - 1);
      // usleep(1000000);

   }
   // return 0;  
}

void lineWrap(char* str, int wrapLen) {
   int lastWrapIdx = 0;
   int k;
   int wrapIdx = 0;

   for(int i = 0; str[i] != '\0'; i++, wrapIdx++) {
      print("enter %d", i);
      if (wrapIdx == wrapLen) {
         // chunk of text over line length
         if (str[i] != ' ') {
            // backtrack to last white space
            for (k = i; k > 0 && str[k] != ' '; k--) {
               continue;
            }
            i = k + 1;
            wrapIdx = 0; 
            print("wrapping at %d", k);
            str[k]='\n';

         } else {
            print("wrapping at %d", i);
            str[i]='\n';
            i++;
            wrapIdx = 0;
         }
      }
   }
}

void centerText(char* str, int size, int disp_size, char* result) {
  int white_spc = disp_size - size;
  int left = white_spc / 2;
  int right = white_spc / 2 + white_spc % 2;
  // char disp_str[16];
  for (int i = 0; i < left; i++) {
    result[i] = ' ';
  }
  for (int i = 0; i < size; i++) {
    result[i + left] = str[i];
  }
  for (int i = 0; i < right; i ++) {
    result[i + left + size] = ' ';
  }
}
