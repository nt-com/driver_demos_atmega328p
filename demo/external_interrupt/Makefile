## Makefile for the UART module test program
## nmt @ NT-COM

CC = avr-gcc
CFLAGS = -Wall -Os

FREQ = -DF_CPU=16000000UL
TARGETMCU = -mmcu=atmega328p

all: ext_int_test ext_int_test.hex

ext_int_test: 
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -o ext_int_test ext_int.c
	
ext_int_test.hex:
	avr-objcopy -O ihex -R .eeprom ext_int_test ext_int_test.hex

clean:
	rm *.hex ext_int_test
