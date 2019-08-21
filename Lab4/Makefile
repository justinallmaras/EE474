all : motor_driver timer_interrupts

motor_driver : motor_driver.o gpio.o
	gcc -Wall -std=gnu99 -o motor_driver motor_driver.o gpio.o

timer_interrupts : timer_interrupts.o
	gcc -Wall -std=gnu99 -o timer_interrupts timer_interrupts.o

motor_driver.o : motor_driver.c motor_driver.h
	gcc -Wall -std=gnu99 -g -c motor_driver.c motor_driver.h

gpio.o : gpio.c motor_driver.h
	gcc -Wall -std=gnu99 -g -c gpio.c motor_driver.h

timer_interrupts.o : timer_interrupts.c timer_interrupts.h
	gcc -Wall -std=gnu99 -g -c timer_interrupts.c timer_interrupts.h

clean :
	rm -f *.o *.gch
