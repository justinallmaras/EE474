

#include "bluetooth.h"


// directory for uart port and baud rate for uart port 
char port[11] = "/dev/ttyO1";
speed_t baud = B115200;


// Polls the bluetooth data directory to see if there is new data and read
// the data into the bluetooth data buffer
int getBluetoothRawData() {
    int i;
    int isData = 0;
    int read_ret = 1;
    memset(buf , '\0' , 16 * sizeof(char));
    int ret = poll(commands , 1 , 500);
    if(ret > 0) {
        for (i = 0; i < 1; i++) {
            // return event for data in
            if(commands[i].revents & POLLIN) {
                print("POLLIN: %d"  , i);
                while(1) {
                    read_ret = read(commands[i].fd, buf, 16);
                    print("READRET: %d STRING: %.*s" , read_ret, read_ret , buf);
                    while(read_ret > 0) {
                        print("READRET: %d STRING: %.*s" , read_ret, read_ret , buf);
                        strncpy(rawData , buf , 16);
                        isData = 1;
                        read_ret = read(commands[i].fd, buf, 16);
                    }
                    print("redret <= 0");
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
                        commands[i].fd = open(port , O_RDONLY | O_NONBLOCK);
                        break;
                    }
                }
            } 
            if((commands[i].revents & POLLHUP) && (read_ret != 0)) {
               print("POLLHUP: %d"  , i);
               close(commands[i].fd);
               commands[i].fd = open(port , O_RDONLY | O_NONBLOCK);
            }
        }
    }
    return isData;
}

// function for getting the raw bluetooth data and doing the basic processing on it
void getBluetoothData (int whatever) {
    print("bluetooth data");
    if(getBluetoothRawData() == 1) {
        char current;
        char joystick [4] = {'\0'};
        int i = 0;
        int x;
        int done = 0;
        while(!done) {
            current = rawData[i];
            if (current == '\0') {
                done = 1;
            } else {
                // Grabs the x and y joystick values
                if(isdigit(current)) {
                    memcpy(joystick , &rawData[i] , 3);
                    bluetoothData[0] = atoi(joystick);
                    i += 3;
                    memcpy(joystick , &rawData[i] , 3);
                    bluetoothData [1] = atoi(joystick);
                    i += 3;
                    print("x: %d" , bluetoothData[0]);
                    print("y: %d" , bluetoothData[1]);
                } else {
                // Sets the modes for the rover
                    switch(current) {
                    case('A'):
                        bluetoothData[2] = 1;
                        i++;
                        break;
                    case('B'):
                        bluetoothData[2] = 0;
                        i++;
                        break;
                    case('C'):
                        bluetoothData[3] = 1;
                        i++;
                        break;
                    case('D'):
                        bluetoothData[3] = 0;
                        i++;
                        break;
                    case('E'):
                        bluetoothData[4] = 1;
                        i++;
                        break;
                    case('F'):
                        bluetoothData[4] = 0;
                        i++;
                        break;
                    case('G'):
                        bluetoothData[5] = 1;
                        i++;
                        break;
                    case('H'):
                        bluetoothData[5] = 0;
                        i++;
                        break;
                    case('I'):
                        bluetoothData[6] = 1;
                        i++;
                        break;
                    case('J'):
                        bluetoothData[6] = 0;
                        i++;
                        break;
                    case('K'):
                        bluetoothData[7] = 1;
                        i++;
                        break;
                    case('L'):
                        bluetoothData[7] = 0;
                        i++;
                        break;
                    default:
                        i++;
                        break;
                    }
                }
            }
            if(i == 16) {
                done = 1;
            }
        }
        bluetoothIsDirty = 1;
    }
}


// configures the uart port for the bluetooth communication
void setupBluetooth() {
    memset(rawData , '\0' , 16 * sizeof(char));
    FILE * sys = fopen("/sys/devices/bone_capemgr.9/slots" , "w");
    fseek(sys , 0 , SEEK_SET);
    fprintf(sys , "%s" , "BB-UART1");
    fflush(sys);
    commands[0].fd = open(port , O_RDONLY | O_NONBLOCK);
    commands[0].events = POLLIN;
    commands[0].revents = 0;
    struct termios settings;
    tcgetattr(commands[0].fd , &settings);
    
    cfsetospeed(&settings , baud);
    settings.c_cflag &= ~PARENB; // no parity
    settings.c_cflag &= ~CSTOPB; // 1 stop bit
    settings.c_cflag &= ~CSIZE;
    settings.c_cflag |= CS8 | CLOCAL; // 8 bits
    settings.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN); // raw mode
    settings.c_oflag &= ~OPOST; // raw output 
    tcsetattr(commands[0].fd , TCSANOW , &settings);
    tcflush(commands[0].fd, TCOFLUSH);
}
