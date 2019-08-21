--------

README.md
Lab 1, EE474 Fall 2016, Bryan Bednarski, Justin Allmaras, Colin Summers

This folder contains all deliverable files for Lab 1 of EE474. Files included are count.c, count,
and makefile. Count.c includes both the seven-count LED and speaker functionality as required in
the lab specification.

PART1: To implement the seven-count feature of this lab, we wrote a program that counts from 0 to
7 in binary (base 2), and displays the current count on a series of three LEDs on the breadboard
of the tank. The first step in doing so involved gaining familiarity with the GPIO pins mounted to
the BeagleBone Black. To control the three LEDS, we open an array of three *sys (system), *dir
(direction) and *val (value) files within device folder for each respective GPIO. Once created, 
with existing initialized values, we are able to set the direction of each pin to "out" in order
to output signals. Then we created a simple loop that counts from 0 to 7 repeatedly,
and passes the binary values of the first, second, and third corresponding binary digit to
its respective LED.

PART2: A pulse width modulation (PWM)  signal is required to drive the breadboard speaker. The period of
this signal (inverse to frequency) ultimatley determines the pitch of the sound created by
the speaker. Therefore, when using the PWM pins on the Beaglebone (different from GPIO pins) we
must write to the device folders that specify the period and duty cycle of the output, in 
nanoseconds. In our program, we define the PWM pins and create these driver folders seperatly from
the GPIO pin setup. Next we set the duty cycle for the defined PWM pin. Within the same loop
as the LED counter we calculate a range of individual periods that correspond to frequencies of 
notes on the chromatic scale. We begin with the A note, a multiple of 440Hz, and use the current
value of the counter to step up the base value by a multiple of 2 ^ (n / 12) -- corresponding to 
the next note on the scale. 

--------- 
