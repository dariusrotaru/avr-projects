# ATmega328p Analog Clock

![C](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=white)
![AVR](https://img.shields.io/badge/AVR-C_Programming-336699?style=for-the-badge)

Fully non blocking, with intrerrupts, modular die roller fo the 1088BS 8x8 Led Matrix written in bare metal C for arduino. (Atmega328p µcontroller)

## Components Used:
* µcontroller: Atmega328P.
* Stepper Motor: 28BYJ-48
* The Driver Module: ULN2003 Driver Board
* additional: jumper wires.

  ### Schematic


## Key Features:
* bare metal C: Basic C, without using any of the Arduino abstractizations and libaries.
* non blocking architecture: Made with a timer driven state machine thus making sure the application is always responsive.

## Code Structure

* 'main.c': here lies the main application logic.
* 'motor.c': self contained library for stepper motor, with complete under the hood functionality.
