/*
Bryan Bednarski, Justin Allmaras, Colin Summers
nunchuck.c
This c file defines two functions that initialize and
read data from a wii nunchuck remote. Data being read
includes 3-axis accelerometer, 2-axis joystick, C and Z buttons.
*/

#include "nunchuck.h"


// configures the I2C port for the nunchuck  controller
int setupNunchuck() {
    // initialization: open port, ioctl address, send 0x40/0x00 init to nunchuck:
    int file = open(PORT, O_RDWR);
    if(fd < 0) error("cant open %s - %m", PORT);
    if(ioctl(file, I2C_SLAVE, ADDR) < 0) error("cant ioctl %s:0x%02x - %m", PORT, ADDR);
    if(write(file, "\x40", 2) < 0) error("cant setup %s:0x%02x - %m", PORT, ADDR);
	return file;
}

// puts the joystick, button, and accelerometer data into a buffer
void get_nunchuck_data(int file) {
	int i= 0;
	int n = 0;
	write(file, "", 1);       							// send a zero to nunchuck to acknowledge the packet
    usleep(100);
	for(i = 0; i < 6; i++) {
		n = read(file, nunchuckBuf+i, 1); 					// read one byte (at index i)
		if(n < 0) error("read error %s:0x%02x - %m", PORT, ADDR);
		if(n == 0) return;
		nunchuckBuf[i] = (nunchuckBuf[i]^0x17)+0x17;  				// decode incoming byte
	}
	
	// 6-byte packet complete
	//for(n=0; n<6; n+1+) printf("%02x ", nunchuckBuf[n]);
	printf("   jx:%-3d  jy:%-3d  ", nunchuckBuf[0], nunchuckBuf[1]);  		// joystick x/y values
	//printf("%c %c  ", nunchuckBuf[5]&1 ? '.' : 'Z', nunchuckBuf[5]&2 ? '.' : 'C');  // keys Z/C
	//for(n=1; n<=3; n++) printf("a%c:%-4d ", 'w'+n,
	//    (nunchuckBuf[n+1]<<2)+((nunchuckBuf[5]>>(n+n))&3));       			// accelerometer x/y/z values
	//printf("\n");	
}

