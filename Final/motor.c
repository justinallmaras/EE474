#include "motor.h"


char motor_port[11] = "/dev/ttyO4";
speed_t motor_baud = B9600;
// neutral serial commands for the motors
unsigned char stopInts[2] = {64 , 192};

// initializes the serial ports for the motor contorller
void setupMotors() {

    FILE * sys = fopen("/sys/devices/bone_capemgr.9/slots" , "w");
    fseek(sys , 0 , SEEK_SET);
    fprintf(sys , "%s" , "BB-UART4");
    fflush(sys);
    usleep(1000);
    motorfd = open(motor_port , O_WRONLY);
    struct termios settings;
    tcgetattr(motorfd , &settings);

    cfsetospeed(&settings , motor_baud);
    settings.c_cflag &= ~PARENB; // no parity
    settings.c_cflag &= ~CSTOPB; // 1 stop bit
    settings.c_cflag &= ~CSIZE;
    settings.c_cflag |= CS8 | CLOCAL; // 8 bits
    settings.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN); // raw mode
    settings.c_oflag &= ~OPOST; // raw output 
    tcsetattr(motorfd , TCSANOW , &settings);
    tcflush(motorfd, TCOFLUSH);

}

// stops the motors
void stop() {
    write(motorfd , stopInts , 2 * sizeof(unsigned char));
}

// writes the values of driveData to the motors checking sensors to avoid collisions
void writeMotors(unsigned char * driveData , int sensors) {
    print("drive data 0: %d" , driveData[0]);
    print("drive data0: %d" , driveData[1]);
    if(driveData[0] > 192 && driveData[1] > 64 && checkSensor(sensors , 0) != 0 ){
      stop();
      return;
    }
    if(driveData[0] < 192 && driveData[1] < 64 && checkSensor(sensors , 1) != 0) {
      stop();
      return;
    }
    if((driveData[0] - 127 < driveData[1]) && checkSensor(sensors , 2) != 0) {
      stop();
      return;
    }
    if((driveData[0] - 127 > driveData[1]) && checkSensor(sensors , 3) != 0) {
      stop();
      return;
    }
    write(motorfd , driveData , 2 * sizeof(unsigned char));
}

// converts the joystick values joyx and joyy to left and right motor values
void pwm_conversion(int nJoyX, int nJoyY, int * mixLeftPtr, int * mixRightPtr) {

  // OUTPUTS
  int nMotMixBuf[2] = { 0 };
  int     nMotMixL;           // Motor (left)  mixed output           (-128..+127)
  int     nMotMixR;           // Motor (right) mixed output           (-128..+127)

  // CONFIG
  // - fPivYLimt  : The threshold at which the pivot action starts
  //                This threshold is measured in units on the Y-axis
  //                away from the X-axis (Y=0). A greater value will assign
  //                more of the joystick's range to pivot actions.
  //                Allowable range: (0..+127)
  float fPivYLimit = 32.0;
      
  // TEMP VARIABLES
  float   nMotPremixL;    // Motor (left)  premixed output        (-128..+127)
  float   nMotPremixR;    // Motor (right) premixed output        (-128..+127)
  int     nPivSpeed;      // Pivot Speed                          (-128..+127)
  float   fPivScale;      // Balance scale b/w drive and pivot    (   0..1   )


  // Calculate Drive Turn output due to Joystick X input
  if (nJoyY >= 0) {
    // Forward
    nMotPremixL = (nJoyX>=0)? 127.0 : (127.0 + nJoyX);
    nMotPremixR = (nJoyX>=0)? (127.0 - nJoyX) : 127.0;
  } else {
    // Reverse
    nMotPremixL = (nJoyX>=0)? (127.0 - nJoyX) : 127.0;
    nMotPremixR = (nJoyX>=0)? 127.0 : (127.0 + nJoyX);
  }

  // Scale Drive output due to Joystick Y input (throttle)
  nMotPremixL = nMotPremixL * nJoyY/128.0;
  nMotPremixR = nMotPremixR * nJoyY/128.0;

  // Now calculate pivot amount
  // - Strength of pivot (nPivSpeed) based on Joystick X input
  // - Blending of pivot vs drive (fPivScale) based on Joystick Y input
  nPivSpeed = nJoyX;
  fPivScale = (abs(nJoyY)>fPivYLimit)? 0.0 : (1.0 - abs(nJoyY)/fPivYLimit);

  // Calculate final mix of Drive and Pivot
  nMotMixL = (1.0-fPivScale)*nMotPremixL + fPivScale*( nPivSpeed);
  nMotMixR = (1.0-fPivScale)*nMotPremixR + fPivScale*(-nPivSpeed);

  // Convert to Motor PWM range
  // (optional)

  *mixLeftPtr = nMotMixL;
  *mixRightPtr = nMotMixR;   

  return;
}

// checks to see if a sensor is returning that there is an object in front
int checkSensor(int sensors , int s) {
    return sensors & (1 << s);
}