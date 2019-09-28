## Makefile for the TWI module test program
## nmt @ NT-COM

CC = avr-gcc
CFLAGS = -Wall -Os

FREQ = -DF_CPU=16000000UL
TARGETMCU = -mmcu=atmega328p

all: twi.o uart.o twi_demo twi_hex

twi.o: twi.c
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -c twi.c

uart.o: uart.c
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -c uart.c

twi_demo: twi.o uart.o twi.h uart.h
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -o twi_demo twi.o uart.o main.c
	
twi_hex:
	avr-objcopy -O ihex -R .eeprom twi_demo twi_demo.hex

clean:
	rm *.hex *.o twi_demo
