// Example ATmega2560 Project
// File: ATmega2560Project.c
// An example file for second year mechatronics project

// include this .c file's header file
#include "Controller.h"

// static function prototypes, functions only called in this file
// ls /dev/tty.*  :command to view MAC com ports
// mode           :command to view Windows com ports

int main(void)
{
  //variable declarations
  
  //timer setup
  //PWM Requirements: 50Hz, pulse duration 1000-2000us

  TCCR1A = (1<<COM1A1);             // Non-inverting PWM on OC1A
  TCCR1B = (1<<WGM13)|(1<<CS11);    // Mode 8, prescaler 8
  ICR1 = 20000;                     // TOP: sets 50Hz frequency
  DDRB |= (1 << PB5);               // Set OC1A (PB5) as output
  OCR1A = 2000;                     // Start at min pulse width (1000us)

  adc_init();

  //main loop
  while(1) 
  {
    uint16_t adcValue = adc_read(0);                  // Read joystick X axis (ADC0)
    OCR1A = 2000 - (adcValue * 2000UL / 1023);        // Map 0-1023 to OCR1A 2000-4000
  }
  return(1);
} // end main 