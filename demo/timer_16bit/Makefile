## Makefile for the UART module test program
## nmt @ NT-COM

CC = avr-gcc
CFLAGS = -Wall -Os

FREQ = -DF_CPU=16000000UL
TARGETMCU = -mmcu=atmega328p

all: timer_16bit_test timer_16bit_test.hex

timer_16bit_test: 
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -o timer_16bit_test timer_16_demo.c
	
timer_16bit_test.hex:
	avr-objcopy -O ihex -R .eeprom timer_16bit_test timer_16bit_test.hex

clean:
	rm *.hex timer_16bit_test
