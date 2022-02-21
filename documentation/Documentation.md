---
title: "MVTHS ROV Documentation"
author: "Robotics Team (2021/2022)"
date: "May 1, 2022"
geometry: margin=1in
---

# Introduction

This is from the first year of MHS/MVTHS being in the MATE ROV competition. It is a ROV designed for dam repair that will likely have its software and hardware modified for use in later ROV competitions. 

# Hardware

## Power Supply

Power is supplied to the ROV from a deep cycle lead acid battery, because it fits the 12v power limit and it is readily available. We will be using deep cycle batteries because we will not be drawing very high current, and deep cycle batteries can be discharged more fully than regular lead acid batteries without being damaged.

16 Gauge stranded wires will be used to supply the ROV as per the motor spec sheet. The stranded wire is chosen because it is more flexible than solid core wire, allowing for better ROV movement.

## Motors 

We are using T200 Thrusters from Blue Robotics, they can draw at maximum 32 amps, however we will **_hopefully_** not be drawing that much current. 

We will have 6 degrees of freedom with thrusters in each respective direction.

The motors are controlled using ESCs, a type of RC controlling commonly used in drones. They allow for variable and reversible speed in the motors using servo controls. 

## Gripper

The gripper is based off of the Newton Subsea Gripper. This design has two bolts loosely gripping each side of the claw, with a servo moving an arm holding the joint between each side of the claw. 

Each of the claws is machined out of aluminum, and has hexagons cut into them for lightness. There are trapezoids on the claws for extra traction. 

## Lights

The lighting is an extremely bright LED. There is a driver connected to the LED to avoid damaging it. There will be aluminum housing surrounding the LED, the housing will be exposed to the water for passive cooling. The front of the housing will be made out of clear acrylic for obvious reasons. 

# Software

## Data Pipeline

Data is sent from the control laptop to the motors and other parts in this order. Sensor data is sent to Arduino over digital pins and the reverse order is used. 

1. Controller data is passed through the laptop into a webclient
2. Webclient uses a websockets connection to the Raspberry Pi(v4)
3. Over a serial connection data is passed to a Arduino through JSON
4. Arduino sends data to ESC using servo library
5. ESC controls motors

## Main Server

The main server is written in python and uses serial connections as well as websockets to relay information from the controlling laptop to the arduino and vice versa. The main server uses asyncio (a library that allows for asynchronous functions in python), meaning that it runs using event based programming and makes heavy use of classes. 

### Camera

The camera is handled inside of the main server, henceforth it is a subsection. The camera takes input from the /dev/video0 file on the raspberry pi using pigame, and makes jpegs in real time. It then sends the jpegs over websockets where it is handled by the webclient which displays it through a html window.

### Sensor Data

The sensors output to either anolog or digital pins on the arduino, then using the ArduinoJson library it makes it into a JSON string that can be sent over serial and then relayed over websockets to the webclient. 

## Webclient

The webclient is the user interface for interacting with the ROV, it is written in raw html/css/js and is held in a file on the computer. The webclient contracts the main server through a websockets connection.

### Controller Input

Input is taken from a PlayStation controller through pygame. It takes velocities from each axis on a coordinate grid. 

### Video Displaying

The video is displayed as a string of JPEGs taken as binary blobs. Every time there is a websocket message event the jpeg will be switched, which is theoretically 30 times a second, but in practice it more resembles 20 depending exposure time required by different lighting conditions. 

\newpage

# Members of the Robotics Team, and Advisors

## Advisors

Thank you to Mr.Christy and Mr.Lewkowitz for taking the time out of their days to supervise us in the shop, and give us assistance in making the ROV.

## Students

All of these students help with and contributed to the ROV, thank you.

* Aaron BenDaniel
* Benjamin Wirz
* Charles Sumorok
* Chris Hunt
* Clark Barayuga
* Cormac Lynch
* Francisco Sono-Ortiz
* Henrik Ronni
* Mathew Harris
* Noah Gibson
* Scott Campbell
* Tanzerul Azam
* Zachary Bertocchi

