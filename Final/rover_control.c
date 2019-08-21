/*
Bryan Bednarski, Justin Allmaras, Colin Summers
rover_control.c
*/

#include "rover_control.h"


// main function for running our rover
int main (int argc , char** argv) {
    setupMotors();
    setupBluetooth();
    setupMP3();
    setupIRSensors();
    nunchuckFile = setupNunchuck();
    MODE = IDLE;
    PREVIOUS_MODE = IDLE;
    left = 0;
    right = 0;
    unsigned char irSensorState = 0;
	int quit = 0;


    runRoverTimerInit();
    //bluetooth_timer_Init();



	while(quit == 0) {
	}

    return 0;
}

void runRover(int whatever) {
    unsigned char motorValues[2];

	// button 6 or no buttons: idle 
    // button 1: bluetooth mode 
    // button 2: nunchuck 
    // button 3: opencv 
    // button 4: enable mp3 
    // button 5: enable openCV
    // set mode
    setMode();
    if (MODE == IDLE) {
        stop();
    } else {
    	switch(MODE) {
            // control logic for bluetooth mode
    		case(BLUETOOTH):
                pwm_conversion(bluetoothData[0] - 200 , bluetoothData[1] - 200 , &motorWriteValues[0] , &motorWriteValues[1]);                   
                motorValues[0] = (unsigned char) (motorWriteValues[0] / 2 + 192);
                motorValues[1] = (unsigned char) (motorWriteValues[1] / 2 + 64);
                writeMotors(motorValues , irSensorState);
    		break;
            // control logic for nunchuck mode
    		case(NUNCHUCK):
                print("\nnunchuck mode\n");
                get_nunchuck_data(nunchuckFile);
                pwm_conversion((nunchuckBuf[0] - 127) / 2 , (nunchuckBuf[1] - 127) / 2 , &motorWriteValues[0] , &motorWriteValues[1]);
                motorValues[0] = (unsigned char) (motorWriteValues[0] / 2 + 192);
                motorValues[1] = (unsigned char) (motorWriteValues[1] / 2 + 64); 
                writeMotors(motorValues , irSensorState);                
    		break;
    		case(OPENCV):


    		break;
            default:
            break;
    	}
    }

    print("Mode : %d" , MODE);
}

// handles the timer interrupt
void interrputHandler(int i) {
    getBluetoothData(i);
    read_all_ADC(i);
    setIRSensorState();
    runRover(i);
}


// initializes a timer interrupt that runs every 40 ms
void runRoverTimerInit() {
   memset (&sa_rover, 0, sizeof (sa));
   sa_rover.sa_handler = &interrputHandler;
   sigaction (SIGVTALRM, &sa_rover, NULL);
   // Configure the timer to expire after 40 msec...
   timer_rover.it_value.tv_sec = 0;
   timer_rover.it_value.tv_usec = 40000;
   // ... and every 40 msec after that. 
   timer_rover.it_interval.tv_sec = 0;
   timer_rover.it_interval.tv_usec = 40000;
   // Start a virtual timer. It counts down whenever this process is executing. 
   if(setitimer (ITIMER_VIRTUAL, &timer_rover, NULL) == 0) {
        print("rover timer setput Successful");
   } else {
        print("rover timer failed");
   }
}

// sets mode based on received button presses from bluetooth app
void setMode() {
    PREVIOUS_MODE = MODE;
	if(bluetoothData[7] == 1) {
		MODE = IDLE;
	} else if(bluetoothData[2] == 1) {
		MODE = BLUETOOTH;
	} else if(bluetoothData[3] == 1) {
        MODE = OPENCV;
	} else if(bluetoothData[4] == 1) {
		MODE = NUNCHUCK;
	} else {
        MODE = IDLE;
    }
}

// sets flags if objects are close for any of the four sensors
void setIRSensorState() {
    int i;
    for(i = 0; i < 4; i++) {
        if(sensorBuf[i] > IR_SENSOR_TRIGGER_VALUE + IR_SENSOR_DEADBAND) {
            irSensorState |= 1 << i;
        } else if (sensorBuf[i] < IR_SENSOR_TRIGGER_VALUE - IR_SENSOR_DEADBAND) {
            irSensorState &= ~(1 << i);
        }
    }
}

