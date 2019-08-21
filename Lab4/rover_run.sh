#!/bin/bash

cd ~/../Lab4/
./motor_driver &
target_PID="$(pgrep -f motor_driver)"
sleep 5
./timer_interrupts $target_PID