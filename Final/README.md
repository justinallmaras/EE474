Bryan Bednarski (1337864), Justin Allmares (1329197), Colin Summers (1332139)
Final Lab README

Compiling using the makefile generates the executeable roverActivate which will allow user control to the rover

rover_control.c: main code that contains all of the decision making for the rover

gpio.c: handles the GPIO pins for simple GPIO writes and reads

irSensors.c:  Handles the read code for the IR distance sensors used on the rover

motor.c:  Handles the motor control code and the conversion from joystick values to motor outputs

mp3.c:  Handles playing mp3 audio when called

nunchuck.c:  Handles the reading of data from a Wii Nunchuck