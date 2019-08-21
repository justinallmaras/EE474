

#include "mp3.h"

// Uses SPI0.0 for data and SPI0.1 for commands

int checkDREQ() {
    unsigned int temp;
    readGPIOVal(115 , &temp);
    return temp;
}

// writes a command to the status registers for the MP3 integrated circuit
void mp3WriteReg (unsigned char addressByte , unsigned char highByte , unsigned char lowByte) {
    setGPIOVal(commandCS , 0);
    while(checkDREQ() == 0) {
        usleep(20);
    }
    unsigned char buf[4];
    // send 0x02 to SPI0.1
    // send address byte
    // send high byte 
    // send low byte
    buf[0] = 0x02;
    buf[1] = addressByte;
    buf[2] = highByte;
    buf[3] = lowByte;
    print("--- %d ---" , write(mp3fd , buf , sizeof(unsigned char) * 4));
    while(checkDREQ() == 0) {
        usleep(1);
    }
    setGPIOVal(commandCS , 1);
}

// sets up the mp3 spi busses
void setupMP3 () {
    //char buf [20] = "BB-SPIDEV0";
    int sysfd = open("/sys/devices/bone_capemgr.9/slots" , O_WRONLY);
    write(sysfd , "BB-SPIDEV0" , 10);
    // wait for spi bus to initialize
    usleep(1000000);
    mp3fd = open("/dev/spidev1.1" , O_RDWR | O_NONBLOCK);
    mp3Datafd = open("/dev/spidev1.0" , O_RDWR | O_NONBLOCK);
    print("fd commands: %d , fd data: %d" , mp3fd , mp3Datafd);
    
    // configure the SPI bus
    //msb first
    //spi mode 0
    //set clock freq to 1 MHz
    //send 0xFF as dummy byte
    uint32_t msb = 1;
    ioctl(mp3Datafd, SPI_IOC_WR_LSB_FIRST, &msb );
    ioctl(mp3fd, SPI_IOC_WR_LSB_FIRST, &msb );
    uint32_t speed = 1000000;
    ioctl(mp3Datafd , SPI_IOC_WR_MAX_SPEED_HZ, &speed );
    ioctl(mp3fd , SPI_IOC_WR_MAX_SPEED_HZ, &speed );
    uint32_t mode = SPI_MODE_0;
    ioctl(mp3Datafd, SPI_IOC_WR_MODE, &mode );
    ioctl(mp3fd, SPI_IOC_WR_MODE, &mode );
    uint32_t bitsPerWord = 8;
    ioctl(mp3Datafd , SPI_IOC_WR_BITS_PER_WORD , &bitsPerWord);
    ioctl(mp3fd , SPI_IOC_WR_BITS_PER_WORD , &bitsPerWord);
    close(sysfd);
    
    commandCS = initGPIO(49 , OUT , 1);
    dreqfp = initGPIO(115 , IN , 0);
    fclose(dreqfp);
    usleep(30);

    //set volume register 0x0B
    //smaller numbers are louder
    print("first write to spi");
    mp3WriteReg(0x0B , 80 , 80);
    print("spi transaction complete");
    while(checkDREQ() == 0) {
        usleep(20);
    }
    // TODO:  add reads to registers
    //set clock freq to 4MHz
    mp3WriteReg(0x03 , 0x60 , 0x00);

    speed = 4000000;
    ioctl(mp3Datafd , SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    // set BBB spi clock to 4 MHz
    // check registers again
}



// plays the track "track2.mp3"
int playTrack () {
    print ("Setting up");
    setupMP3();
    print("done with setup");

    print("storing file to buffer");

    FILE *fp;
    unsigned char *buffer;

    fp = fopen ( "track2.mp3" , "rb" );
    if( !fp ) {
      perror("track2.mp3"),exit(1);
    }

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) {
      fclose(fp),fputs("memory alloc fails",stderr),exit(1);
    }

    /* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) ) {
      fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
    }
    fclose(fp);
    print("done storing file to buffer");

    p = buffer;
    j = 0;

    return 0;
}


// pushes data to the data buffer of the mp3 integrated circuit
void pushData(int whatever) {
    if(checkDREQ() == 1) {
        if(j < lSize - 32) {
          write(mp3Datafd , p , 32 * sizeof(unsigned char));
          p += 32;
          j += 32;
        }
    }
}
