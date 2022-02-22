#!/bin/bash

arduino-cli compile -b arduino:avr:uno
arduino-cli -b arduino:avr:uno upload -p /dev/ttyUSB0 temperature.ino
screen /dev/ttyUSB0
