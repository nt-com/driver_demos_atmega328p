## Makefile for the UART module test program
## nmt @ NT-COM

CC = avr-gcc
CFLAGS = -Wall -Os

FREQ = -DF_CPU=16000000UL
TARGETMCU = -mmcu=atmega328p

all: uart uart_test uart_hex

uart: uart.c
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -c uart.c

uart_test: uart.o uart.h uart_cfg.h
	$(CC) $(CFLAGS) $(FREQ) $(TARGETMCU) -o uart_test uart.o main.c
	
uart_hex:
	avr-objcopy -O ihex -R .eeprom uart_test uart_test.hex

clean:
	rm *.hex *.o uart_test
