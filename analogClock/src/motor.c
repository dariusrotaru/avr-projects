#include "avr/io.h"
#include "motor.h"


#define HIGH 1
#define LOW 0
#define INTERNAL_DELAY 3

static const uint8_t  inPins[4]={PB0, PB1, PB2, PB3};
static volatile uint8_t* const inPorts[4]={&PORTB, &PORTB, &PORTB, &PORTB};
uint8_t Seq[8][4]= {
    {HIGH,  HIGH,  LOW,  LOW},
    { LOW,  HIGH,  LOW,  LOW},
    { LOW,  HIGH, HIGH,  LOW},
    { LOW,   LOW, HIGH,  LOW},
    { LOW,   LOW, HIGH, HIGH},
    { LOW,   LOW,  LOW, HIGH},
    {HIGH,   LOW,  LOW, HIGH},
    {HIGH,   LOW,  LOW,  LOW}
};
static unsigned long motorPreviousMilis = 0;
static uint16_t stepsRemaining = 0;

/*
    In1 = PB0
    .........
    In4 = PB3
*/
void MotorInit(){
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
}
/**
 * @brief Rotates the motor by a single step.
 * @note Internal "static" function, only to be caleld by MotorUpdate().
 * It drives the motor by half-steps, by turnin 2 coils at a time, then only the last coil turned,
 * and so on.
 */
static void MotorStep(){
    static uint8_t phase = 0;
    uint8_t i;
    phase = (phase + 7) % 8;
    for(i  = 0; i < 4; i++){
        if(Seq[phase][i] == HIGH){
            *inPorts[i] |= (1 << inPins[i]);
        } else {
            *inPorts[i] &= ~(1 << inPins[i]);
        }
    }
}

void MotorUpdate(unsigned long currentMilis){
    if((stepsRemaining > 0) && (currentMilis - motorPreviousMilis >=INTERNAL_DELAY)){
        motorPreviousMilis = currentMilis;
        MotorStep();
        stepsRemaining--;
    }
}

void MotorRotateSteps(uint16_t steps){
    stepsRemaining = steps;
}

int MotorIsBusy(void){
    return (stepsRemaining >0);
}