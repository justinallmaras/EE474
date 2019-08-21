
README.md

Lab 3 README file
Bryan Bednarski (1337864), Justin Allmares (1329197), Colin Summers (1332139)

This repository contains our c programs for the EE474 Lab 3: LKM's and Shift Registers. The files included in this repository are built around our KERNAL module for the beaglebone black microcontroller. The KERNAL module allows for device drivers to be written, loaded and run independently within the KERNAL, as needed for our tank's specified functionallity. To compile the kernal module, unpackage the KERNAL.tar.gz compression folder, scp program files to the beaglebone and compile/run. Once running, .ko device drivers were written using kernal space syntax to drive the LCDS and the H-bridge motor controls. The kernal module file for the LCD display is called new_char.ko and the module for the motor driver is called motor_driva.ko. The files written for each of these are described below:

LCD Driver:
C code for our LCD driver is included in the new_char.c program file. This file contains the instantiation of all functions for the LCD display, including the semaphore, __init__, __exit__ and licensing information for the kernal module. These functions support the printing and scrolling of text on the LCD display by the user and can be run simultaneously to the motor kernel.

Motor Driver:
Code for our motor driver functionality is included in the motor_driva.c program. Similarly to the LCD display, this file defines all functionality for the motor driver. Define by a case statement, the drive mode is set when the user writes to the /dev/motor_driva file. Characters associated with different modes are as follows:

F: forward
R: reverse
L: Turn left in place
R: turn right in place

A script has been included to compile and load kernal modules to the beaglebone kernal. Run the push.sh script followed by a list of which modules to include and kernel modules will be pushed from the host linux computer, accross a ssh connection to the beaglebone and begin functioning.




