all : jitter LCDScreen pipeWriter

jitter : gpio.o jitter.o
	gcc -Wall -std=gnu99 -o jitter jitter.o gpio.o

LCDScreen : LCD_control.o LCD.o gpio.o
	gcc -Wall -std=gnu99 -o LCDScreen LCD_control.o LCD.o gpio.o

pipeWriter : pipe_write.o
	gcc -Wall -std=gnu99 -o pipeWriter pipe_write.o

LCD.o : LCD.c LCD.h gpio.o
	gcc -Wall -std=gnu99 -g -c LCD.h LCD.c gpio.h

gpio.o : gpio.c gpio.h
	gcc -Wall -std=gnu99 -g -c gpio.c gpio.h

LCD_control.o : LCD_control.c LCD.o
	gcc -Wall -std=gnu99 -g -c LCD_control.c LCD.h

jitter.o : gpio.o jitter.c
	gcc -Wall -std=gnu99 -g -c jitter.c gpio.h

pipe_write.o : pipe_write.c pipe_write.h
	gcc -Wall -std=gnu99 -g -c pipe_write.c pipe_write.h	

clean :
	rm -f *.o *.gch
