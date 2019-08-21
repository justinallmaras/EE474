Lab 3 README file
Bryan Bednarski (1337864), Justin Allmares (1329197), Colin Summers (1332139)

This is the README for Lab4: Interrupts and Realtime

motor_control.c - Contains main logic of motor controller. Initializes GPIO ports and PWM for motor controller.
Defines event handler for interrupts from IR sensors. Intializes ADC ports for reading joystick position. 

motor_control.h - header file for motor_control.c

timer_interrupts.c - Contains main logic for reading IR sensor data. Initializes ADC input for reading
IR sensor values. Intializes timer interupt used to poll IR sensors at predefined interval. Sends
interrupt signal to motor_control runtime when IR sensor is below predefined threshhold.

timer_interrupts.h - header file for timer_interrupts.c

rover_run.h - bash script which runs timer_interrupt and motor_control. Passes PID of motor_control to timer_interrupt
so that interrupt signal can be sent to motor_control process.

gpio.c - Defines several library functions for initialziing GPIO ports and interfacing with them

gpio.h - header file for gpio.c
