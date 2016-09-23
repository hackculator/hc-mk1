#!/usr/bin/python3

import RPi.GPIO as GPIO #importing the GPIO library

GPIO.setmode(GPIO.BOARD) #selecting the numbering of the GPIO on the Pi
gpio=12  #Selecting the GPIO pin that is going to be in high at start

GPIO.setup(gpio,GPIO.OUT) 
GPIO.output(gpio,True)

