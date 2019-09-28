/*********************************************************************
 * External Interrupts with GPIO - Demo Program
 * Short Name: ext_int
 * Author: nmt @ NT-COM
 * Date: 21.09.2019
 * Description: configures one pin as an ouput, a second pin as an 		
 *							input and enables external interrupts on this pin
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [21.09.2019][nmt]: initial commit
 *********************************************************************/

/*********************************************************************
 * Usage: 
 *
 * Connect a LED to pin D6 on the arduino, a button on pin D2.
 * once the button is pressed an interrupt is triggered that 
 * turns on the LED.
 *********************************************************************/

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*********************************************************************
 * MAIN FUNCTION
 *********************************************************************/ 
int main(void) {

		/* ouput configuration */

		/* set pin D6 as an output using the data direction register */
		DDRD |= (1 << DDD6);
		/* set the initial state of the output to low */
		PIND &= ~(1 << PIND6);	
		
		/* input configuration */

		/* set pin D2 as an input */
    DDRD &= ~(1 << DDD2); 
		/* activate the internal pullup for pin D2 */
    PORTD |= (1 << PORTD2); 
	
		/* interrupt configuration */

		/* EICRA is the external interrupt control register,
			 table 12-1 in the ATmega328p datasheet shows all
			 configuration options. Because we activated a pullup
			 for pin D2, we can set this to trigger an interrupt 
			 on a falling edge */
    EICRA |= (1 << ISC11);    
		/* this is the external interrupt mask register, here
			 we enable interrupt request 0 */
    EIMSK |= (1 << INT0); 

		/* this function activates global interrupts */
    sei();                    

    while(1) {
			/* turn LED off continuously */
			PIND &= ~(1 << PIND6);	
    }
}

/*********************************************************************
 * INTERRUPT SERVICE ROUTINES
 *********************************************************************/ 
ISR (INT0_vect) {
	/* once the interrupt occurs the LED is turned on */
	PIND |= (1 << PIND6);
}


