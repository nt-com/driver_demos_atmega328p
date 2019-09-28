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
 * Connect a LED to pin D6 on the arduino, that way you can see 
 * the PWM at work.
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

#define DUTY_CYCLE_MAX			255
#define INITIAL_DUTY_CYCLE	128

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
 * @param initial_duty_cycle the initial duty cycle value [0-255]
 * @return void
 */
void pwm_setup(uint8_t initial_duty_cycle);

/**
 * @brief sets the PWM duty cycle
 * @param duty_cycle the duty cycle value [0-255]
 * @return void
 */
void pwm_set_duty_cycle(uint8_t duty_cycle);

/*********************************************************************
 * MAIN FUNCTION
 *********************************************************************/ 
int main(void) {

	uint8_t ui8_loop = 0x00;

	/* initialize peripherals */

	/* the OCR0A register for PWM is at the package pin D6 for the arduino,
		 this pin outputs the PWM */
	gpio_setup();

	pwm_setup(INITIAL_DUTY_CYCLE);

	/* global interrupt enable */
  sei();        

    while (1) {
			/* SUPERLOOP */
	
			/* we are going to manipulate the duty cycle every 5 ms
				 the LED will become brighter until the maximum value is 
				 reached, after that it will turn off briefly starting to
				 get brighter again */
			for(ui8_loop = 0; ui8_loop < DUTY_CYCLE_MAX; ui8_loop++) {
				pwm_set_duty_cycle(ui8_loop);
				_delay_ms(5);
			}

    }
}

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/ 
void gpio_setup() {

	/* gpio setup */

	/* pin D6 to output */
	DDRD = (1 << DDD6);

}

void pwm_setup(uint8_t duty_cycle) {

	/* pwm setup */

	/* NOTE:
		 We are setting up fast PWM mode below. */

	/* set the initial duty cycle value */
	OCR0A = duty_cycle;

	/* here we can set what happens to OC0A on a compare match
		 we will clear the register, corresponding to non-inverting mode */
	TCCR0A |= (1 << COM0A1); 
	/* this activates fast PWM, according to table 14-8 in the 
		 ATmega328p datasheet */
	TCCR0A |= (1 << WGM00 | (1 << WGM01)); 
	/* this sets the prescale factor to 8 */
	TCCR0B |= (1 << CS01);
	
}

void pwm_set_duty_cycle(uint8_t duty_cycle) {
	OCR0A = duty_cycle;
}


