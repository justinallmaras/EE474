/* 
 * new_char.h: holds all include statements, preprocessor constants
 *             data structures, global variables, and function prototypes
 *             used throughout this file.
 *
 * general rule:
 * global variables should be declared static,
 * including global variables within the file.
 */
#ifndef NEW_CHAR_H_
#define NEW_CHAR_H_
 

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/unistd.h>

#define DEVICE_NAME "new_char"

/* data structures */
// contains data about the device.
// data : buffer for character data stored.
// sem  : semaphore.
struct fake_device {
	char data[1000];
	struct semaphore sem;
} virtual_device;

    // setup pins
    int RSPin = 49;
    int EPin = 112;
    int RWPin = 115;
    int RCLKPin = 67;
    int SERPin = 66;
    int SRCLKPin = 69;
    // File pointer declarations for LCD.cgedfi

    // function prototypes for new_char.c
    static void init_all_gpio_pins(void); 
    static void init_gpio_pin(int pin_num);
    static void exit_gpio_pin(int pin_num);
    static void exit_all_gpio_pins(void);
    static void initLCD(void);
    static void clockStrobe(void);
    static void sendData(int data , int readNWrite , int regSelect);
    static void sendChar(char c);
    static void cursorShiftLeft(int spaces);
    static void cursorShiftRight(int spaces);    
    static void setCursorTopRow(void);
    static void setCursorBottomRow(void);
    static void clearDisplay(void);
    static void printLCD(const char * top , const char * bottom);
    static void shiftRegisterCLK(int pin);
    static int dev_release();

/* global variables */
// stores info about this char device.
static struct cdev* mcdev;
// holds major and minor number granted by the kernel
static dev_t dev_num;

/* function prototypes */
// file operations
static int __init driver_entry(void);
static void __exit driver_exit(void);
static int  device_open(struct inode*, struct file*);
static int device_close(struct inode*, struct file *);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);

/* operations usable by this file. */
static struct file_operations fops = {
   .owner = THIS_MODULE,
   .write = device_write,
   .open = device_open,
   .release = device_close
};
#endif
