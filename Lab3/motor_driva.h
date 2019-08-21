/* 
 * new_char.h: holds all include statements, preprocessor constants
 *             data structures, global variables, and function prototypes
 *             used throughout this file.
 *
 * general rule:
 * global variables should be declared static,
 * including global variables within the file.
 */
#ifndef MOTOR_DRIVA_H
#define MOTOR_DRIVA_H
 
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

#define DEVICE_NAME "motor_drive"

/* data structures */
// contains data about the device.
// data : buffer for character data stored.
// sem  : semaphore.
struct fake_device {
    char data[32];
    struct semaphore sem;
} virtual_device;

   // funciton prototypes
   static void init_all_gpio_pins(void); 
   static void init_gpio_pin(int pin_num);
   static void exit_gpio_pin(int pin_num);
   static void exit_all_gpio_pins(void);
   static int dev_release(void);
   static void turn_right(void);
   static void turn_left(void);
   static void forward(void);
   static void backward(void);
   static void setA(int a1 , int a2 , int pwm);
   static void setB(int b1 , int b2 , int pwm);
   static void stop(void);


   // GPIO pins used
   int AIN1 = 68;
   int AIN2 = 45;
   int BIN1 = 44;
   int BIN2 = 26;
   int standby = 47;
   int PWMA = 46;
   int PWMB = 27;



/* function prototypes */
static int __init driver_entry(void);
static void __exit driver_exit(void);
static int  device_open(struct inode*, struct file*);
static int device_close(struct inode*, struct file *);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);

/* operations usable by this file. */
static struct file_operations fops = {
   .owner = THIS_MODULE,
   .read = device_read,
   .write = device_write,
   .open = device_open,
   .release = device_close
};
#endif
