
#include "irSensors.h"

char dir[40];
int state = 0;

// sets up the analog to digital conversion directories
void setupIRSensors () {
    int fd;
    fd = open("/sys/devices/bone_capemgr.9/slots" , O_WRONLY);
    write(fd , "cape-bone-iio" , 13);
    close(fd);
    while(fd = open("/sys/devices/ocp.3/helper.15/AIN0" , O_RDONLY) < 0);
    close(fd);
}

// reads the for infrared sensors and stores their values in a buffer
void read_all_ADC(int whatever) {
	for(int i = 0; i < 4; i++) {
	    char value[4];
	    char dir [40] = {'\0'};
	    sprintf(dir , "/sys/devices/ocp.3/helper.15/AIN%d" , i);
	    int analogfd = open(dir , O_RDONLY);
	    read(analogfd , value , 4 * sizeof(char));
	    print("%s" , value);
	    sensorBuf[i] = atoi(value);
	    close(analogfd);
	}
	return;
}

