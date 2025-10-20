#include <avr/io.h>

#include <stdlib.h>

#include <avr/interrupt.h>

#include <stdbool.h>

#include "matrix.h"

#define BTN PC5
#define DIE 20
#define ANIMSTART 21
#define ANIMEND 24
#define CRITFAIL 25
#define CRITSUCC 26

#define STATE_IDLE 0
#define STATE_ANIMATING 1
#define STATE_SHOWING_RESULT 2
#define STATE_CRIT_FAIL 3
#define STATE_CRIT_SUCCESS 4

uint8_t gCurrentState = STATE_IDLE;
uint32_t gLastActionTime = 0;
uint32_t gAnimationStartTime = 0;
uint8_t gAnimationFrame = 0;
uint8_t gDieResult = 0;

#define DEBUG 0

volatile uint32_t gMilis = 0;
volatile bool gBtnPressed = false;
volatile uint32_t gLastDebounceTime = 0;

void Timer0Init() {
  TCCR0A |= (1 << WGM01);
  OCR0A = 249;
  TIMSK0 |= (1 << OCIE0A);
  TCCR0B |= (1 << CS01) | (1 << CS00);
}

void BtnIntrerruptInit() {
  PCICR |= (1 << PCIE1);

  PCMSK1 |= (1 << PCINT13);
}

uint16_t AdcRead(uint16_t channel) {
  ADMUX = (1 << REFS0) | (channel & 0x07);
  ADCSRA = (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADC;
}

int main(void) {
    MatrixInit();
    Timer0Init();
    BtnIntrerruptInit();
    srand(AdcRead(4));
    sei();

    MatrixLoadPattern(27);

    while (1) {

        if (gBtnPressed) {
            gBtnPressed = false;
            gCurrentState = STATE_ANIMATING;
            gAnimationFrame = ANIMSTART;
            MatrixLoadPattern(gAnimationFrame);
            gAnimationStartTime = gMilis;
            gLastActionTime = gMilis;
        }

        switch (gCurrentState) {
            case STATE_IDLE:
                break;
            case STATE_ANIMATING:
                if (gMilis - gLastActionTime >= 100) {
                    gLastActionTime = gMilis;
                    gAnimationFrame++;
                    if (gAnimationFrame > ANIMEND) gAnimationFrame = ANIMSTART;
                    MatrixLoadPattern(gAnimationFrame);
                }
                if (gMilis - gAnimationStartTime > 1500) {
                    gDieResult = rand() % (DIE + 1);
                    MatrixLoadPattern(gDieResult);
                    gLastActionTime = gMilis;
                    gCurrentState = STATE_SHOWING_RESULT;
                }
                break;
            case STATE_SHOWING_RESULT:
                if (gMilis - gLastActionTime >= 3000) {
                    if (gDieResult == 1) { gCurrentState = STATE_CRIT_FAIL; MatrixLoadPattern(CRITFAIL); }
                    else if (gDieResult == DIE) { gCurrentState = STATE_CRIT_SUCCESS; MatrixLoadPattern(CRITSUCC); }
                    else { gCurrentState = STATE_IDLE; MatrixLoadPattern(0); }
                    gLastActionTime = gMilis;
                }
                break;
            case STATE_CRIT_FAIL:
            case STATE_CRIT_SUCCESS:
                if (gMilis - gLastActionTime >= 2000) {
                    gCurrentState = STATE_IDLE;
                    MatrixLoadPattern(27);
                }
                break;
        }
        MatrixUpdate();
    }
}

ISR(TIMER0_COMPA_vect) {
  gMilis++;
}
ISR(PCINT1_vect) {
    if (gMilis - gLastDebounceTime > 50) {
        if (!(PINC & (1 << BTN))) {
            gBtnPressed = true;
        }
    }
    gLastDebounceTime = gMilis;
}
