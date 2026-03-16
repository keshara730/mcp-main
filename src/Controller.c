// Example ATmega2560 Project
// File: ATmega2560Project.c
// An example file for second year mechatronics project

// include this .c file's header file
#include "Controller.h"

// static function prototypes, functions only called in this file
// ls /dev/tty.*  :command to view MAC com ports
// mode           :command to view Windows com ports

// variable declaration
volatile uint16_t lastMilliseconds = 0; // only used in main
volatile uint16_t milliseconds2 = 0; // 
volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t running = 0;

int main(void)
{
  cli();  // disable global interrupts
  
  serial0_init();

  // timer setup
  TCCR1A = 0;                                   // WGM11=0, WGM10=0 (CTC Mode)
  TCCR1B = (1<<WGM12)|(1<<CS11)|(1<<CS10);      // CTC mode 4, prescaler /64
  OCR1A  = 249;                                 // TOP value for 1ms period
  TIMSK1 = (1<<OCIE1A);                         // Enable output compare A interrupt
  
  // button setup
  DDRD  &= ~((1<<PD2)|(1<<PD3));                // Set INT0 and INT1 pins as inputs
  // PORTD |=  ((1<<PD2)|(1<<PD3));             // RC debounce, internal pullup not needed
  EICRA |=  (1<<ISC21);                         // INT2 falling edge, set ISC21
  EICRA &= ~(1<<ISC20);                         // clear ISC20
  EICRA |=  (1<<ISC31);                         // INT3 falling edge, set ISC31
  EICRA &= ~(1<<ISC30);                         // clear ISC30
  EIMSK |=  (1<<INT2)|(1<<INT3);                // Enable INT2 and INT3

  sei(); // enable global interrupts
  
  // main loop
  while(1)
  {
    if(milliseconds2 != lastMilliseconds) // only update when time changes
    {
      lastMilliseconds = milliseconds2;

      // format and print as MM:SS:mmm
      char timeString[12];
      sprintf(timeString, "%02d:%02d:%03d\r", minutes, seconds, milliseconds2);
      serial0_print_string(timeString);
    }
  }
  return(1);
} // end main 

ISR(TIMER1_COMPA_vect)
{
  if(running)
  {
    milliseconds2++;
    if(milliseconds2 >= 1000)
    {
      milliseconds2 = 0;
      seconds++;
      if(seconds >= 60)
      {
        seconds = 0;
        minutes++;
        if(minutes >= 60)
        {
          minutes = 0; // rollover at 60 minutes
        }
      }
    }
  }
}

ISR(INT2_vect) // start/stop button
{
  running ^= 1; // (0->1 or 1->0)
}

ISR(INT3_vect) // reset button
{
  running = 0;
  milliseconds2 = 0;
  seconds = 0;
  minutes = 0;
}