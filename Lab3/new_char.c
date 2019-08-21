/* 
 * new_char.c: holds a buffer of 100 characters as device file.
 *             prints out contents of buffer on read.
 *             writes over buffer values on write.
 *
 * Notes: usleep range bug if upper bound too low?
 */
#include "new_char.h"
#define ROW_SIZE 16
/********************* FILE OPERATION FUNCTIONS ***************/


static int majorNumber;
static struct class * lcdClass = NULL;
static struct device * lcdDevice = NULL;


// runs on startup
// intializes module space and declares major number.
// assigns device structure data.
// Makes device dev file /dev/new_char
static int __init driver_entry(void) {
        printk(KERN_INFO "Initializing LCD\n");
        majorNumber = register_chrdev(0 , DEVICE_NAME , &fops);
        if (majorNumber < 0) {
          printk(KERN_ALERT "LCD failed to acquire major number\n");
          return majorNumber;
        }
        printk(KERN_INFO "LCD registered with major number: %d\n" , majorNumber);
        
        lcdClass = class_create(THIS_MODULE , "classLCD");
        if (IS_ERR(lcdClass)) {
           unregister_chrdev(majorNumber , DEVICE_NAME);
           printk(KERN_ALERT "Failed to create class\n");
           return PTR_ERR(lcdClass);
        }

        lcdDevice = device_create(lcdClass , NULL , MKDEV(majorNumber , 0) , NULL , DEVICE_NAME);
        if (IS_ERR(lcdDevice)) {
           class_destroy(lcdClass);
           unregister_chrdev(majorNumber , DEVICE_NAME);
           printk(KERN_ALERT "Failed to create device\n");
           return PTR_ERR(lcdDevice);
        }
        // intitialize semaphore
        sema_init(&virtual_device.sem, 1);
        


	       //initialzie gpio pins and LCD
        init_all_gpio_pins();   
	      initLCD();
        gpio_set_value(61, 1);
        gpio_set_value(RCLKPin , 0);
        gpio_set_value(SERPin , 0);
        gpio_set_value(SRCLKPin , 0);
        printk(KERN_INFO "LKM created successfully\n");

	return 0;
}

// function to initialize all gpio pins used by the kernel module
static void init_all_gpio_pins(void) {
    init_gpio_pin(SERPin);
    init_gpio_pin(SRCLKPin);
    init_gpio_pin(RCLKPin);
    init_gpio_pin(RSPin);
    init_gpio_pin(EPin);
    init_gpio_pin(RWPin);

}

// function to initialize one gpio pin to an output
static void init_gpio_pin(int pin_num) {
	if (gpio_is_valid(pin_num)) {
		gpio_request(pin_num, "sysfs");
		gpio_direction_output(pin_num, 0);
		gpio_export(pin_num, 0);
	}
}

// this function sends 1 byte of serial data across GPIO pin to shift register
void sendData(int data , int writeNRead , int regSelect) {
    printk(KERN_INFO "data: %x writeNRead: %d regSelect %d", data, writeNRead, regSelect);
    int dataEightBit;
    gpio_set_value(RSPin, regSelect);
    gpio_set_value(RWPin, writeNRead);
    int divisor = 128;
    dataEightBit = data % 256;
    for(int i = 0; i < 8; i++) {
        gpio_set_value(SERPin , (dataEightBit / divisor) % 2);
        shiftRegisterCLK(SRCLKPin);
        divisor /= 2;
    }
    shiftRegisterCLK(RCLKPin);
    usleep_range(2, 40);
    clockStrobe();
    usleep_range(40, 80);
}

// sends a clock strobe to either of the shift register clocks based on the parameter
static void shiftRegisterCLK (int pin) {
    gpio_set_value(pin , 1);
    usleep_range(10 , 20);
    gpio_set_value(pin , 0);
    usleep_range(10 , 20);
}

static void sendChar(char c) {
       sendData(c, 0, 1);
}

// initialize the LCD display.  Call before writing data to the LCD
static void initLCD(void) {
   // wait more than 15ms after power on before initializing
   msleep(20); // 20ms
   // send 0011xxxx - function set command
   sendData(0x30 , 0 , 0);
   // wait at least 4.1ms
   msleep(5);
   // send 0011xxxx
   sendData(0x30 , 0 , 0);
   // wait at least 100 microseconds
   usleep_range(150, 500);
   // function set with number of lines and font select
   sendData(0x30 , 0 , 0);
   // function set - number of lines (2) and display font (5x10) can be sent
   usleep_range(40, 80);
   sendData(0x3C , 0 , 0);
   // delays to wait for possible busy flag. Consider using gpio input to check busy flag 
   usleep_range(40, 80);
   // Display OFF
   sendData(0x08 , 0 , 0);
   // clear display
   sendData(0x01 , 0 , 0);
   usleep_range(40, 80);
   // entry set mode
   sendData(0x06 , 0 , 0);
   usleep_range(40, 80);
   // display ON with cursor and blink
   sendData(0x0F , 0 , 0);
   usleep_range(40, 80);
}

// Clock strobe function necessary for sending characters to the data pins.
// FUnciton is used in sendData() function below
static void clockStrobe() {
   gpio_set_value(EPin, 1);
   // wait at least 450 ns
   usleep_range(1, 40);
   gpio_set_value(EPin, 0);
   usleep_range(1, 40);
}

// Shifts cursor left by the number of spaces passed to function
static void cursorShiftLeft(int spaces) {
   int i;
   for(i = 0; i < spaces; i++) {
      sendData(0x10 , 0 , 0);
   }
}

// Shifts cursor right by the number of spaces passed to the function
static void cursorShiftRight(int spaces) {
   int i;
   for(i = 0; i < spaces; i++) {
      sendData(0x14 , 0 , 0);
   }
}

// Clears display for next string to be written
static void clearDisplay () {
   sendData(0x01 , 0 , 0);
   msleep(16);
}

// Moves cursor to bottom row 
static void setCursorBottomRow() {
   sendData(0xC0 , 0 , 0);
   msleep(5);
}

// Moves cursor to top row
static void setCursorTopRow() {
   sendData(0x80 , 0 , 0);
   msleep(5);
}

static void exit_gpio_pin(int pin_num) {
    gpio_unexport(pin_num);
    gpio_free(pin_num);
}

static void exit_all_gpio_pins(void) {
    gpio_set_value(61, 0);
    exit_gpio_pin(RSPin);
    exit_gpio_pin(EPin);
    exit_gpio_pin(RWPin);
    exit_gpio_pin(SRCLKPin);
    exit_gpio_pin(RCLKPin);
    exit_gpio_pin(SERPin);
    exit_gpio_pin(61);
}

// Prints current buffered string of chatacters to the LCD screen
// using the sendChar function
static void printLCD(const char * top , const char * bottom) {
   //clear display
   //return cursor home
   clearDisplay();
   int i;
   // Prints to top row
   setCursorTopRow();
   for(i = 0; i < 16; i++) {
      if(top[i] != '\0') {
	   sendChar(top[i]);
	  } else {
       sendChar(' ');
	  }
   }
   // Prints to bottom row
   setCursorBottomRow();   
   for(i = 0; i < 16; i++) {
      if(bottom[i] != '\0') {
	  sendChar(bottom[i]);
	  } else {
      sendChar(' ');
      }
   }
}

// called up exit.
// unregisters the device and all associated gpios with it.
static void __exit driver_exit(void) {
        exit_all_gpio_pins();
        dev_release();
	device_destroy(lcdClass , MKDEV(majorNumber , 0));
        class_unregister(lcdClass);
        class_destroy(lcdClass);
        unregister_chrdev(majorNumber , DEVICE_NAME);
        //cdev_del(mcdev);
	//unregister_chrdev_region(dev_num, 1);
	printk(KERN_ALERT "new_char: successfully unloaded\n");
}

static int dev_release(struct inode *inodep , struct file *filp) {
   printk(KERN_INFO "LCD Device closed successfully\n");
   return 0;
}

// Called on device file open
//	inode reference to file on disk, struct file represents an abstract
// checks to see if file is already open (semaphore is in use)
// prints error message if device is busy.
int device_open(struct inode *inode, struct file* filp) {
	//if (down_interruptible(&virtual_device.sem) != 0) {
		//printk(KERN_ALERT "new_char: could not lock device during open\n");
		//return -1;
	//}
        //if(mutex_trylock(&lcd_mutex) == 0) {
        //}
	printk(KERN_INFO "new_char: device opened\n");
	return 0;
}

// Called upon close
// closes device and returns access to semaphore.
int device_close(struct inode* inode, struct  file *filp) {
	//up(&virtual_device.sem);
	printk(KERN_INFO "new_char, closing device\n");
	return 0;	
}

// Called when user wants to send info to device
static ssize_t device_write(struct file* filp, const char* bufSource, size_t bufCount, loff_t* curOffset) {
	//printk(KERN_INFO "new_char: writing to device...\n");
	//int ret = copy_from_user(virtual_device.data, bufSource, bufCount);
        //sendChar(virtual_device.data[0]);
	//memset(virtual_device.data, '\0' , 100);
        //sprintf(virtual_device.data , "%s" , bufSource);
        
	
	//memcpy(virtual_device.data, bufSource, bufCount);
	
	int amtWrote = 0;	

	printk(KERN_INFO "%s", virtual_device.data);
  printk(KERN_INFO "Received %d size buffer\n", bufCount);
        
	for (int i = 0; (i < bufCount); i++) {
		virtual_device.data[i] = bufSource[i];
		printk(KERN_INFO "wrote %c\n", virtual_device.data[i]);
		amtWrote++;
	}
	
  char bottom[16] = {0};
  char top[16] = {0};
  for (int amtPrinted = 0; amtPrinted < amtWrote; amtPrinted += ROW_SIZE) {
    memcpy(top, bottom, ROW_SIZE);
    int diff = amtWrote - amtPrinted;
    if (diff < 16) {
      memcpy(bottom, &virtual_device.data[amtPrinted], diff);
      memset(bottom + diff, ' ', ROW_SIZE - diff);
    } else {
    memcpy(bottom, &virtual_device.data[amtPrinted], ROW_SIZE);
    }
    printk(KERN_INFO "printed %.*s\n%.*s\n", ROW_SIZE, top, ROW_SIZE, bottom);
    printLCD(top, bottom);
    msleep(1500);
  }  

	return amtWrote;
}


MODULE_LICENSE("GPL"); // module license: required to use some functionalities.
module_init(driver_entry); // declares which function runs on init.
module_exit(driver_exit);  // declares which function runs on exit.
