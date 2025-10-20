# Modular Dice Roller

![C](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=white)
![AVR](https://img.shields.io/badge/AVR-C_Programming-336699?style=for-the-badge)

Fully non blocking, with intrerrupts, modular die roller fo the 1088BS 8x8 Led Matrix written in bare metal C for arduino. (Atmega328p µcontroller)

## Components Used:
* µcontroller: Atmega328P.
* display: 8x8 common Anode Led Matrix 1088BS.
* input: a simple push button
* additional: x2 220Ω resitors.

  ### Schematic

  ![Schematic](DiceRoller/hardware/Schematic.pdf)


## Key Features:
* bare metal C: Basic C, without using any of the Arduino abstractizations and libaries.
* non blocking architecture: Made with a timer driven state machine thus making sure the application is always responsive.
* uses intrerrupts: Used a milis() system tick for time managemnt.

## Code Structure

* 'main.c': here lies the main application logic.
* 'matrix.c': self contained library for the led matrix, which is used to initialize and display the patterns.
