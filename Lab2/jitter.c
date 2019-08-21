#include <stdio.h>
#include <time.h>
#include "gpio.h"
#include <unistd.h>

int main() {
	 FILE *gpio = initGPIO(61, OUT, 0);
	 int goHigh = 0;
	 while(1) {
//            usleep(1);
	    if (goHigh == 1) {
               setGPIOVal(gpio, 1);
	        goHigh = 0; 
            } else { // goHigh == 0
	        setGPIOVal(gpio, 0);
	        goHigh = 1;
	    }
  	 }
	 return 0;
}
