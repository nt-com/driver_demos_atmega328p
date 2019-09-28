/*********************************************************************
 * UART Interface  Driver - Main File
 * Short Name: uart
 * Author: nmt @ NT-COM
 * Date: 15.06.2019
 * Description: a simple echo program that sends a welcome string
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [15.09.2019][nmt]: initial commit
 *********************************************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

/* uncomment if you want to use the alternative receive function */
/* #define RECV_ALT 1 */

/* MAIN */
int main () {

	uint8_t ui8_data = 0x00; /* stores the received data */
	uint8_t test_string[7] = { 'n', 't', '-', 'c', 'o', 'm', '\n' };
	uint8_t echo_string[6] = { 'e', 'c', 'h', 'o', ':', ' ' };
	uint8_t newline = '\n';

	/* initialization of the interface */
	uart_init();
	
	/* send a string to check if this function works */
	uart_send_string(&test_string[0], 7);
	
	while(1) {
		
		/* SUPERLOOP */	
		/* realizes a single character echo */
		
		/* chooses between the recv and alternative recv functions */
		#ifndef RECV_ALT
			ui8_data = uart_recv();
		#else
			uart_recv_alt(&ui8_data);
		#endif
		
		/* send a prompt indicating that the echoed character will
		 * follow */
		uart_send_string(&echo_string[0], 6);
		/* send the data we received */
		uart_send(ui8_data);
		/* send a newline so everything looks nicer */
		uart_send(newline);

	} /* while(1)*/

}

