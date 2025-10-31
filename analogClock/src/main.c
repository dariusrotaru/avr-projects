#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "motor.h"

#define TWELVE_MINUTES_MS 720000UL
#define STEPS 68 // 1/60th of a full roation (~4096 / 60), 60 = 12(hours) * 5(increments per).
volatile unsigned long gMilis = 0;

void Timer0Init(){
  //ctc mode
  //TCCR0A
  //  7      6      5      4    3 2   1     0
  //COM0A1 COM0A0 COM0B1 COM0B0 – – WGM01 WGM00
  TCCR0A |= (1 << WGM01);
  //TCCR2B
  //  7     6   5 4   3    2    1    0
  //FOC0A FOC0B – – WGM02 CS02 CS01 CS00
  TCCR0B |= (1 << CS01) | (1 << CS00);
  OCR0A = 249; // for 1ms
  //7 6 5 4 3   2      1      0
  //– – – – – OCIE0B OCIE0A TOIE0
  TIMSK0 |= (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect){
  gMilis++;
}

int main(){
  unsigned long previousMilis = 0;
  MotorInit();
  Timer0Init();
  sei();
  while (1){
    unsigned long currentMilis;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
      currentMilis = gMilis;
    }
    //12 min check
    if(currentMilis - previousMilis >= TWELVE_MINUTES_MS){
      previousMilis = currentMilis;
      if(!MotorIsBusy()){
        MotorRotateSteps(STEPS);
      }
    }
    MotorUpdate(currentMilis);
  }
  return 1;
}