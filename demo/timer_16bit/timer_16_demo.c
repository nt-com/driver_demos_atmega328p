/*********************************************************************
 * 16-bit Timer Driver - Demo Program
 * Short Name: timer_8bit
 * Author: nmt @ NT-COM
 * Date: 20.09.2019
 * Description:		sets up the 16-bit timer, triggers an interrupt on
 *								compare match
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [20.09.2019][nmt]: initial commit
 *********************************************************************/

/*********************************************************************
 * Usage: 
 *
 * Connect a LED to pin B1 on the arduino, that way you can see 
 * when a timer interrupt occurs. 
 *********************************************************************/

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

/*********************************************************************
 * MACROS
 *********************************************************************/ 
/* set the CPU frequency to 4 MHz */
#ifndef F_CPU
	#define F_CPU 4000000UL
#endif

#define COUNTER_COMPARE_VALUE	0x3333

/*********************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************/ 
/**
 * @brief sets up the pin B1 to connect an LED
 * @return void
 */
void gpio_setup(void);

/**
 * @brief sets up the timer
 * @param comp_value the value to compare the counter value of the timer to
 * @return void
 */
void timer_16_setup(uint16_t comp_value);

/*********************************************************************
 * MAIN FUNCTION
 *********************************************************************/ 
int main(void) {

	/* initialize peripherals */
	gpio_setup();
	timer_16_setup(COUNTER_COMPARE_VALUE);

	/* global interrupt enable */
  sei();        

    while (1) {
			/* SUPERLOOP */
    }
}

/*********************************************************************
 * INTERRUPT SERVICE ROUTINE
 *********************************************************************/ 
/* ISR triggered on timer1 compare match */
ISR (TIMER1_COMPA_vect) {
	
	/* toggles the second bit in the register PINB points to, thus
		 pin B1 is toggled. Note that '^' is not necessary, '=' handles
		 the toggling. */	
	PINB = 0x02;	

}

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/ 
void gpio_setup() {

	/* gpio setup */

	/* pin B1 to output */
	DDRB = (1 << DDB1);
	/* set the pin to low */
	PINB &= ~(1 << PINB1);

}

void timer_16_setup(uint16_t comp_value) {

	/* timer setup */

	/* NOTE: check out datasheet section 14 for more details */

	/* NOTE:	This is the "Clear Timer on Compare Match (CTC) Mode" 
						basically you set a value in OCR0A and the timer value 
						is continuously compared to this value. Once it matches
						the counter of the timer is cleared. When this happens
						you can trigger an interrupt, which is exactly what we
						are setting up below. To change what happens when an 
						interrupt occurs add your code to the ISR above. */

	/* this value is calculated as follows: 
		 f_OCnA = CPU_CLOCK_FREQ / ( 2*PRESCALE_VALUE*(1+OCRnA) ) */	
	OCR1A = comp_value;

	/*	this register is used with register TCCR1A to set counting sequence,
			the maximum value for the counter and wave form generation. In this 
			case, we are setting up CTC mode and checking OCR1A if we have a 
			match. If you look in the datasheet table 15-5 describes this. */
	TCCR1B |= (1 << WGM12);	

	/* activate interrupt on compare match */
	TIMSK1 |= (1 << OCIE1A);

	/* set the prescaler to 1024 */
	TCCR1B |= ( (1 << CS12) | (1 << CS10) );

}


