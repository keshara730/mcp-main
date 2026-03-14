//Example ATmega2560 Project
//File: ATmega2560Project.c
//An example file for second year mechatronics project

//include this .c file's header file
#include "Robot.h"

volatile uint8_t button_pressed = 0;

ISR(INT2_vect) {
    button_pressed = 1;
}

void init_interrupt() {
    // Set INT2 as input
    DDRD &= ~(1 << DD2);
    
    // Trigger on falling edge (ISC21=1, ISC20=0)
    EICRA |= (1 << ISC21);
    EICRA &= ~(1 << ISC20);
    
    // Enable INT2
    EIMSK |= (1 << INT2);
    
    // Enable global interrupts
    sei();
}

int main(void)
{
  serial0_init();
  adc_init();
  init_interrupt();
  _delay_ms(20);

  char buffer[32];

 // cli();
 // EICRA |= (1<<ISC21);
 // EIMSK |= (1<<INT2);
 // DDRD  &= ~(1 << DD2);   // set PD2 as input
// PORTD |=  (1 << PD2);   // enable internal pull-up
 // sei();

  while(1) //main loop
  {
    if (button_pressed) {
      uint16_t light = adc_read(0);  
      sprintf(buffer, "\nLight level: %d/1023\n", light);
      serial0_print_string(buffer);
      button_pressed = 0;  // reset the flag
    }
  }
  return(1);
}//end main 
