# HUDplug
Arduino based simulator I/O. With x-plane plugin for x-plane 10 and 11+.

## Early development phase
The program is in very early development. Only the basic functions of digital in and out are working at the moment. But the framework underneath is ready for more features.

### Working features
* Digital input, on/off switch and push and hold to inc/dec value
* Analog input
* TM1673 function to display number on 7 seg display. 
* Servo output
* PWM output
* 4x4 key matrix
* Ability to have more than one arduino for input/output through serial communication. 

## Installation
* Download and unzip the latest release zip file with the x-plane plugin in your xplane plugins folder (X-Plane 11/Resources/plugins)
* Create a config file (X-Plane 11/Resources/plugins/HUDplug/config.txt) (https://github.com/condac/HUDplug/wiki/Example-config)
* Download and install Arduino program if you do not have it. ( It is needed to compile and upload firmware to your arduino)
* Download the latest source code in the master branch. There you find the iotest arduino project.
* Select the right arduino board and upload the program to your arduino.


## Goals
The goal with this project is to create a hardware layer that can chain multiple Arduinos together with serial communications to create a larger network. It allows you to have any kind of Arduino in the chain, lets say you have an DUE to do something it can relay it back to a master device that is a different kind, MEGA or UNO for example.

I also want to keep the code simple to make it easy for hoppyist like my self to contribute to this project. Code readability is prefered over performance in the early phase of this project.

The Arduinos should relay the information back to the PC without doing anything with the information to make things simple in the Arduino parts and to save on program memory. If for example a analog value need to be trimmed or calibrated it should be done in the plugin that relay the information to the game or software on the PC side.

## Questions or suggestions
Feel free to open a issue for anything from questions to feature requests. 

## Donations
Feel free to make any donations on my patreon page https://www.patreon.com/condac or with paypal https://www.paypal.me/condac
