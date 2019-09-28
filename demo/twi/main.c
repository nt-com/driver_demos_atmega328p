/*********************************************************************
 * Twin Wire Interface (TWI) Driver - Demo Program
 * Short Name: twi
 * Author: nmt @ NT-COM
 * Date: 19.09.2019
 * Description:		interfaces with the MPU6050 sensor, sends the raw 
 *								accelerometer z-axis data over uart 
 *********************************************************************/

/*********************************************************************
 * Changelog:
 * [Date][Author]:[Change]
 * [19.09.2019][nmt]: initial commit
 *********************************************************************/

/*********************************************************************
 * Usage:
 *					Connect	the MPU6050 sensor to the arduino, AD0 is not set in 
 *					this example, so the I2C address of the MPU6050 is 0x68.
 *					Please note that this program sends the raw sensor values
 *					over the UART.
 *********************************************************************/

/*********************************************************************
 * LIBRARIES
 *********************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "uart.h"
#include "twi.h"

/*********************************************************************
 * MACROS
 *********************************************************************/
/* MPU6050 wake up status */
#define MPU6050_WAKEUP_SUCCESS 0x00
#define MPU6050_WAKEUP_FAILURE 0x01

/* relevant register address of the MPU6050 for this program */
#define MPU6050_ADDR 0b11010000 /* 7-bit I2C address (110 1000 = 0x68)+ write bit */
#define MPU6050_POWER_REGISTER 0x6b
#define MPU6050_ACC_Z_HIGH_REGISTER 0x3f
#define MPU6050_ACC_Z_LOW_REGISTER 0x40

/* delay between sensor readings in ms */
#define READ_DELAY 100

/*********************************************************************
 * TYPES
 *********************************************************************/
/* error codes for return values from the twi */
enum TWI_STATUS_CODES {
	OK,
	FAILURE_TW_START,
	FAILURE_TW_MT_SLA_ACK,
	FAILURE_TW_MT_DATA_ACK,
	FAILURE_TW_REP_START,
	FAILURE_TW_MR_SLA_ACK,
	FAILURE_TW_MR_DATA_NACK	
};

/* states for the main state machine to read the MPU6050 and send
   the measured values over the UART */
enum MAIN_FSM_STATES {
	STATE_WAIT,
	STATE_TWI_READ_HIGH,
	STATE_TWI_READ_LOW,
	STATE_UART_SEND_HIGH,
	STATE_UART_SEND_LOW,
	STATE_UART_SEND_NEWLINE,
	STATE_ERROR
};


/*********************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************/

/* NOTE: these functions send error codes using the UART if an
				 error occurs. */

/**
 * @brief wakes up the MPU6050
 * @return OK on success, error code otherwise
 */
uint8_t MPU6050_wakeup();

/**
 * @brief reads a single byte from the MPU6050
 * @param data stores the data that was read
 * @param addr register address to read from
 * @return OK on success, error code otherwise
 */
uint8_t MPU6050_read(uint8_t *data, uint8_t addr);


/*********************************************************************
 * MAIN FUNCTION
 *********************************************************************/
void main() {

	/* state variable for the main state machine */
	uint8_t main_state = STATE_WAIT;

	/* store accelerometer values */
	uint8_t acc_z_high = 0x00;
	uint8_t acc_z_low = 0x00;
	/* newline character for UART */
	uint8_t newline = '\n';

	/* initialize UART and TWI */
	twi_init();
	uart_init();

	/* power on the MPU6050 */
	if(MPU6050_wakeup() == OK) {
		/* send zero over the UART to indicate that the MPU6050 is activated */
		uart_send(MPU6050_WAKEUP_SUCCESS);
	} else {
		uart_send(MPU6050_WAKEUP_FAILURE);
		/* if the MPU6050 can not be activated indicate and error using the UART 
			 and loop forever */
		while(1);
	}

	while(1) {

		/* SUPERLOOP */

		/* state machine for interaction with sensor and UART */
		switch(main_state) {

			case STATE_WAIT:							/* wait for READ_DELAY between sensor readings */
																		_delay_ms(READ_DELAY);
																		acc_z_high = 0x00;
																		acc_z_low = 0x00;
																		main_state = STATE_TWI_READ_HIGH;
																		break;
			case STATE_TWI_READ_HIGH:			/* read the accelerometer z-axis high byte */
																		if(MPU6050_read(&acc_z_high, MPU6050_ACC_Z_HIGH_REGISTER)) {
																			/* if an error occurs go into the error state */
																			main_state = STATE_ERROR;
																		}
																		main_state = STATE_TWI_READ_LOW;
																		break;
			case STATE_TWI_READ_LOW:			/* read the accelerometer z-axis low byte */
																		if(MPU6050_read(&acc_z_low, MPU6050_ACC_Z_LOW_REGISTER)) {
																			/* if an error occurs go into the error state */
																			main_state = STATE_ERROR;	
																		}
																		main_state = STATE_UART_SEND_HIGH;
																		break;
			case STATE_UART_SEND_HIGH:		/* send the high byte over the UART */
																		uart_send(acc_z_high);
																		main_state = STATE_UART_SEND_LOW;
																		break;
			case STATE_UART_SEND_LOW:			/* send the low byte over the UART */
																		uart_send(acc_z_low);
																		main_state = STATE_UART_SEND_NEWLINE;
																		break;
			case STATE_UART_SEND_NEWLINE:	/* send a newline over the UART */
																		uart_send(newline);	
																		main_state = STATE_WAIT;
																		break;
			case STATE_ERROR:							uart_send(newline);
																		break;
			default:											/* default state should never occur */
																		main_state = STATE_ERROR;
																		break;
		}; /* switch case*/

	} /* while(1) */

} /* main */

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

uint8_t MPU6050_wakeup() {

	uint8_t error_code = OK;

	twi_start();
	if(twi_status() != TW_START) {
		error_code = FAILURE_TW_START;
		uart_send(error_code);
		goto stop;
	}
	/* send address */
	twi_write((MPU6050_ADDR));
	/* wait for ACK */
	if(twi_status() != TW_MT_SLA_ACK) {
		error_code = FAILURE_TW_MT_SLA_ACK;
		uart_send(error_code);
		goto stop;
	}
	
	/* write operation */
	/* power register access*/
	twi_write(MPU6050_POWER_REGISTER);
	if(twi_status() != TW_MT_DATA_ACK) {
		error_code = FAILURE_TW_MT_DATA_ACK;
		uart_send(error_code);
		goto stop;
	}
	
	/* set sleep to zero */
	twi_write(0x00);
	if(twi_status() != TW_MT_DATA_ACK) {
		error_code = FAILURE_TW_MT_DATA_ACK;
		uart_send(error_code);
		goto stop;
	}
	
stop:

	twi_stop();
	
	if(error_code != OK) {
		return 1;
	}

	return 0;
	
}

uint8_t MPU6050_read(uint8_t *data, uint8_t addr) {

	uint8_t error_code = OK;

	*data = 0x00;

	twi_start();
	if(twi_status() != TW_START) {
		error_code = FAILURE_TW_START;
		uart_send(error_code);
		goto stop;
	}
	/* send address */
	twi_write((MPU6050_ADDR));
	/* wait for ACK */
	if(twi_status() != TW_MT_SLA_ACK) {
		error_code = FAILURE_TW_MT_SLA_ACK;
		uart_send(error_code);
		goto stop;
	}

	/* register we want to read from */
	twi_write(addr);
	/* wait for ACK */
	if(twi_status() != TW_MT_DATA_ACK) {
		error_code = FAILURE_TW_MT_DATA_ACK;
		uart_send(error_code);
		goto stop;
	}
	
	/* repeated start condition */
	twi_start();
	if(twi_status() != TW_REP_START) {
		error_code = FAILURE_TW_REP_START;
		uart_send(error_code);
		goto stop;
	}
	
	/* read operation */
	twi_write((MPU6050_ADDR) | 1);
	if(twi_status() != TW_MR_SLA_ACK) {
		error_code = FAILURE_TW_MR_SLA_ACK;
		uart_send(error_code);
		goto stop;
	}
	
	/* get data and tell sensor we want no more */
	*data = twi_read_nack();
	if(twi_status() != TW_MR_DATA_NACK) {
		error_code = FAILURE_TW_MR_DATA_NACK;
		uart_send(error_code);
		goto stop;
	}
	
stop:

	twi_stop();
	
	if(error_code != OK) {
		return 1;
	}
	
	return 0;
	
}


