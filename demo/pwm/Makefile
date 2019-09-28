## Makefile for the UART module test program
## nmt @ NT-COM

CC = avr-gcc
CFLAGS = -Wall -Os

FREQ = -DF_CPU=16000000UL
TARGETMCU = -mmcu=atmega328p

all: pwm_test pwm_test.hex

pwm_test: 
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -o pwm_test pwm_demo.c
	
pwm_test.hex:
	avr-objcopy -O ihex -R .eeprom pwm_test pwm_test.hex

clean:
	rm *.hex pwm_test
