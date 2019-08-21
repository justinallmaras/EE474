--------

README.md
Lab 2, EE474 Fall 2016, Bryan Bednarski, Justin Allmaras, Colin Summers

This folder contains all deliverable files for Lab 2 of EE474. Under the same makefile, these files will be compiled into three separate executable programs called "jitter", "LCDScreen", and "pipeWriter." The function of these programs is described below:

Jitter: The jitter program simply flips a GPIO pin high and low with a 1uSec sleep between call between value changes. This process allows us to observe the effects of scheduler jitter on the execuation of this continuously looping instruction. We were able to capture this sceduler jitter on the oscilloscope and include that result in our lab report.

LCDScreen: The LCDScreen program represents our LCD screen driver program that continousouly runs on the beaglebone and awaits input from the two data pipes that passes the driver strings of character data. The driver consists of the LCD.c,  GPIO.c and LCD_control.c files. LCD.C contains functions that intialize gpio pins, the LCD screen and define abstracted LCD scrren functions. GPIO.c is an individual GPIO dirver that allows the LCD driver to simply call for a GPIO to be intialized, rather than writing to individual GPIO data and value folders. Lastly, the LCD_control file instantiates and reads from two data pipes. The first data pipe allows the user to write into the command prompt and directly to our LCD screen in what we define as "FREE" mode. The second data pipe is used to read 16 character strings from a text file and print them on the LCD screen.

pipeWriter: The pipWriter program is run on top of an existing LCDScreen program to pass text strings to the respective two pipes being read from by the LCDScreen program. We created a function within pipe_write.c that parses through the text file passed to it and displays these text chunks.

--------- 
