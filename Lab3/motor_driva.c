#include "motor_driva.h"

static int majorNumber;
static struct class * motorClass = NULL;
static struct device * motorDevice = NULL;


// function called when the driver is inserted into the kernel
static int __init driver_entry(void) {
   printk(KERN_INFO "Initializing motor driver\n");
   majorNumber = register_chrdev(0 , DEVICE_NAME , &fops);
   if (majorNumber < 0) {
         printk(KERN_ALERT "Motor driver failed to acquire major number\n");
         return majorNumber;
      }
   printk(KERN_INFO "Motor driver registered with major number: %d\n" , majorNumber);
        
   motorClass = class_create(THIS_MODULE , "classMotor");
   if (IS_ERR(motorClass)) {
      unregister_chrdev(majorNumber , DEVICE_NAME);
      printk(KERN_ALERT "Failed to create class\n");
      return PTR_ERR(motorClass);
   }

   motorDevice = device_create(motorClass , NULL , MKDEV(majorNumber , 0) , NULL , DEVICE_NAME);
   if (IS_ERR(motorDevice)) {
      class_destroy(motorClass);
      unregister_chrdev(majorNumber , DEVICE_NAME);
      printk(KERN_ALERT "Failed to create device\n");
      return PTR_ERR(motorDevice);
   }
   
   // intialize gpio pins and set standby to 0 to avoid any spurious turn on
   init_all_gpio_pins();
   gpio_set_value(standby , 0);
   gpio_set_value(AIN1 , 0);
   gpio_set_value(AIN2 , 0);
   gpio_set_value(PWMA , 0);
   gpio_set_value(BIN1 , 0);
   gpio_set_value(BIN2 , 0);
   gpio_set_value(PWMB , 0);
   // enable the motor driver
   gpio_set_value(standby , 1);
   return 0;
}

// function to intitialize all gpio pins
static void init_all_gpio_pins(void) {
   init_gpio_pin(AIN1);
   init_gpio_pin(AIN2);
   init_gpio_pin(BIN1);
   init_gpio_pin(BIN2);
   init_gpio_pin(standby);
   init_gpio_pin(PWMA);
   init_gpio_pin(PWMB);
}

// initiaizes one GPIO pin to an output
static void init_gpio_pin(int pin_num) {
   if (gpio_is_valid(pin_num)) {
      gpio_request(pin_num, "sysfs");
      gpio_direction_output(pin_num, 0);
      gpio_export(pin_num, 0);
   }
}

   static void exit_all_gpio_pins(void) {
      exit_gpio_pin(AIN1);
      exit_gpio_pin(AIN2);
      exit_gpio_pin(BIN1);
      exit_gpio_pin(BIN2);
      exit_gpio_pin(standby);
      exit_gpio_pin(PWMA);
      exit_gpio_pin(PWMB);
   }

   static void exit_gpio_pin(int pin_num) {
      gpio_unexport(pin_num);
      gpio_free(pin_num);
   }


// function called at end of operation to free all associations
static void __exit driver_exit(void) {
   exit_all_gpio_pins();
   dev_release();
   device_destroy(motorClass , MKDEV(majorNumber , 0));
   class_unregister(motorClass);
   class_destroy(motorClass);
   unregister_chrdev(majorNumber , DEVICE_NAME);

   printk(KERN_ALERT "motor_drive: successfully unloaded\n");
}

static int dev_release(void) {
   printk(KERN_INFO "Motor Device closed successfully\n");
   return 0;
}


int device_open(struct inode *inode, struct file* filp) {
   //if (down_interruptible(&virtual_device.sem) != 0) {
      //printk(KERN_ALERT "new_char: could not lock device during open\n");
      //return -1;
   //}
   printk(KERN_INFO "motor_drive: device opened\n");
   return 0;
}

static ssize_t device_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset) {
   printk(KERN_INFO "new_char: Reading from device...\n");
   return copy_to_user(bufStoreData, virtual_device.data, bufCount);
}

int device_close(struct inode* inode, struct file *filp) {
   printk(KERN_INFO "motor drive: closing device\n");
   return 0;
}

// Called when user wants to send info to device
static ssize_t device_write(struct file* filp, const char* bufSource, size_t bufCount, loff_t* curOffset) {
   //printk(KERN_INFO "new_char: writing to device...\n");
   //int ret = copy_from_user(virtual_device.data, bufSource, bufCount);
   //sendChar(virtual_device.data[0]);
   sprintf(virtual_device.data , "%s" , bufSource);
   printk(KERN_INFO "Received %d size buffer\n" , bufCount);
   switch(virtual_device.data[0]) {
      case 'F':
         forward();
      break;
      case 'B':
         backward();
      break;
      case 'L':
         turn_left();
      break;
      case 'R':
         turn_right();
      break;
      default:
         stop();
      break;

   }
   return strlen(virtual_device.data);
   // print buffer, then clear the buffer, then return ret
}

static void turn_right() {
   setA(0 , 1 , 1);
   setB(1 , 0 , 1);
}

static void turn_left() {
   setA(1 , 0 , 1);
   setB(0 , 1 , 1);
}

static void forward() {
   setA(1 , 0 , 1);
   setB(1 , 0 , 1);
   
}

static void backward() {
   setA(0 , 1 , 1);
   setB(0 , 1 , 1);
}

static void setA(int a1 , int a2 , int pwm) {
   gpio_set_value(AIN1 , a1);
   gpio_set_value(AIN2 , a2);
   gpio_set_value(PWMA , pwm);
}

static void setB(int b1 , int b2 , int pwm) {
   gpio_set_value(BIN1 , b1);
   gpio_set_value(BIN2 , b2);
   gpio_set_value(PWMB , pwm);
}

static void stop() {
   setB(0 , 0 , 0);
   setA(0 , 0 , 0);
}

MODULE_LICENSE("GPL");
module_init(driver_entry);
module_exit(driver_exit);
