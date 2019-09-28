/*********************************************************************
 * 8-bit Timer Driver - Demo Program
 * Short Name: timer_8bit
 * Author: nmt @ NT-COM
 * Date: 15.06.2019
 * Description:		sets up the 8-bit timer, triggers an interrupt on
 *								compare match
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [15.09.2019][nmt]: initial commit
 *********************************************************************/

/*********************************************************************
 * Usage: 
 *
 * Connect an LED to pin B1 on the arduino, that way you can see 
 * when a timer interrupt occurs. The LED will blink pretty fast
 * though because it is only an 8-bit timer.
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

#define COUNTER_COMPARE_VALUE	0xff

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
void timer_setup(uint8_t comp_value);

/*********************************************************************
 * MAIN FUNCTION
 *********************************************************************/ 
int main(void) {

	/* initialize peripherals */
	gpio_setup();
	timer_setup(COUNTER_COMPARE_VALUE);

	/* global interrupt enable */
  sei();        

    while (1) {

			PINB &= ~(1 << PB1);
			_delay_ms(1000);

    }
}

/*********************************************************************
 * INTERRUPT SERVICE ROUTINE
 *********************************************************************/ 
/* ISR triggered on timer0 compare match */
ISR (TIMER0_COMPA_vect) {
	/* set the pin to high */
	PINB |= (1 << PINB1);
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

void timer_setup(uint8_t comp_value) {

	/* timer setup */

	/* NOTE: check out datasheet section 14 for more details */

	/* NOTE:	This is the "Clear Timer on Compare Match (CTC) Mode" 
						basically you set a value in OCR0A and the timer value 
						is continuously compared to this value. Once it matches
						the counter of the timer is cleared. When this happens
						you can trigger an interrupt, which is exactly what we
						are setting up below. To change what happens when an 
						interrupt occurs add your code to the ISR above. */
	
	/* set CTC mode */
	TCCR0A |= (1 << WGM01);
	/* set value to count to */
	OCR0A = comp_value;
	/* enable the timer/counter0 compare match A interrupt */
	TIMSK0 |= (1 << OCIE0A);
	/* set the prescaler to 1024, resulting in a slower timer 
	   so we can actually see the led blinking */
	TCCR0B |= ((1 << CS00) | (1 << CS02));
  
}


