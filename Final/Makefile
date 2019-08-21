all: roverActivate

roverActivate : rover_control.o bluetooth.o nunchuck.o motor.o gpio.o mp3.o
	gcc -Wall -std=gnu99 -o roverActivate rover_control.o bluetooth.o nunchuck.o motor.o gpio.o mp3.o

rover_control.o : rover_control.c rover_control.h bluetooth.h mp3.h
	gcc -Wall -std=gnu99 -g -c rover_control.c rover_control.h bluetooth.h mp3.h 

bluetooth.o : bluetooth.c bluetooth.h
	gcc -Wall -std=gnu99 -g -c bluetooth.c bluetooth.h

motor.o : motor.c motor.h gpio.h
	gcc -Wall -std=gnu99 -g -c motor.c motor.h gpio.h

nunchuck.o : nunchuck.c nunchuck.h
	gcc -Wall -std=gnu99 -g -c nunchuck.c nunchuck.h 

gpio.o : gpio.c gpio.h
	gcc -Wall -std=gnu99 -g -c gpio.c gpio.h

mp3.o : mp3.c mp3.h gpio.h
	gcc -Wall -std=gnu99 -g -c mp3.c mp3.h gpio.h

clean :
	rm -f *.o *.gch
