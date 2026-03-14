//Example ATmega2560 Project
//File: ATmega2560Project.c
//An example file for second year mechatronics project

//include this .c file's header file
#include "Controller.h"

//static function prototypes, functions only called in this file

int main(void)
{
  DDRA = 0xFF;             // put PORTA into output mode (for LEDs)
  DDRH &= ~(1 << PH4);    // PH4 as input for push button
  
  PORTH = 0;               // set output of PORTH low
  PORTH |= (1<<PH4);        // enable internal pull up resistor

  adc_init();              // initialse ADC
  _delay_ms(20);           // wait for a few clock cycles

  uint8_t channel = 0;

  uint8_t barTable[9] = {
        0b00000001,          // 0 LEDs
        0b00000011,          // 1 LED
        0b00000111,          // 2 LEDs
        0b00001111,          // 3 LEDs
        0b00011111,          // 4 LEDs
        0b00111111,          // 5 LEDs
        0b01111111,          // 6 LEDs
        0b11111111,          // 7 LEDs
    };

  while(1)                 // main loop
  {
    if(!(PINH & (1 << PH4)))
        {
            channel = !channel;   // toggle between VRX (0) and VRY (1)
        }

        // read ADC and display on LEDs
        uint16_t adcVal = adc_read(channel);   // read VRx or VRy (0-1023)
        uint8_t numLEDs = (adcVal >> 2) / 32;  // scale to 0-7
        PORTA = barTable[numLEDs];             // display bar graph on LEDs
    }
  return(0);

} //end main 