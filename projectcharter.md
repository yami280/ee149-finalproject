## Project Title: **Home Automation System** (Name Pending)
## Team: *Jene Li, Michelle Nguyen, Jacob Minyoung Huh*
## EECS 149/249A Project Charter, Fall, 2014

## **Project Goal**
The goal of this project is to create a networked home automation system that contains sensors that interact with each other and work together to create useful datasets. A secondary goal of this project is to solve some of the problems with home automation system adoption that many implementations face today. 

## **Project Approach**
This project will consist of three main modules. We will develop a backend software layer module to facilitate the collaboration of the sensors and control of the actuators. There will be a communications layer that hooks together the various network interfaces such as Bluetooth, XBee, NFC, RFID, or WiFi to send messages to and from the software layer. Lastly, we will be implementing one sensor/actuator pair as a proof of concept of this central home automation hub design.

## **Resources**
After some research, we have decided that we will use the Intel Galileo microprocessor along with some smaller cheaper boards to create the array of sensor/actuator devices. We need to ensure that all the boards have wireless connectivity in order to connect to the cloud. Afterwards, we will look into what sensors we wish to add onto each device. The current plan is to include visual and pressure sensors. Another idea for sensor actuator pairs is a door sensor that detect the identity of whoever walks through based on Bluetooth LE connections. This will trigger an event within the main software layer to send to the individual. 

## **Schedule**
* October 21: Project charter (this document)
* October 28: Choice of platform and sensor/actuators finalized after discussion with GSIs.
* November 4: State machine model for tasks with logic and timing
* November 11: Installed software for development, experimented with boards
* November 18: Mini project update: Demonstrate sensor/actuator action on one task
* Novemeber 25: Configure network communications properly with server running
* December 2: Control and configure devices via wireless working 
* December 9: Multiple tasks working, testing in progress
* December 16: Demo video made, powerpoint prepared
* December 17: Final presentation and demo
* December 19: Project report and video turned in

## **Risk and Feasibility**
There are many unknowns in measuring various household tasks. One implementation is a trash detecting device. This have variability in weight and possibility for false positives. Also, the parts have to be within the budget and this may be hard for multiple devices. Networking this may be difficult, depending on which board we choose. 