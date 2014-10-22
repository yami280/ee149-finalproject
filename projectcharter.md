## Project Title: **Home Automation System** (Name Pending)
## Team: *Jene Li, Michelle Nguyen, Jacob Minyoung Huh*
## EECS 149/249A Project Charter, Fall, 2014

## **Project Goal**
The goal of this project is to create a network of simple sensors and actuators that attempt to solve many of the problems associated with managing a living space. We will focus our design on a system that is easily accessible, compatible with a wide array of hardware devices, and expandable.

## **Project Approach**
The project will be comprised of many small microcontrollers with various sensors relaying information to a webserver hosted on the cloud. Each microcontroller will contain a subset of sensors and actuators that can be configured to perform multiple home automation tasks.

## **Resources**
After some research, we have decided that we will use the Intel Galileo microprocessor along with some smaller cheaper boards to create the array of sensor/actuator devices. We need to ensure that all the boards have wireless connectivity in order to connect to the cloud. Afterwards, we will look into what sensors we wish to add onto each device. The current plan is to include visual and pressure sensors.

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