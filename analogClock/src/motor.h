#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

/**
 * @brief Initializes the motor driver pins (PB0-PB3) as outputs.
 */
void MotorInit();
/**
 * @brief The Motor's main state machine update function.
 * @param currentMilis which is the current system time, gotten from gMilis.
 */
void MotorUpdate(unsigned long currentMilis);
/**
 * @brief Sets the number o steps the motor is required to "step".
 * @param steps The number of steps to rotate.
 */
void MotorRotateSteps(uint16_t steps);
/**
 * @brief Checks the current state of the motor(if it is busy or not).
 * @return int 1 (true) if the motor is busy (moving), or 0 if it is idle.
 */
int MotorIsBusy(void);

#endif