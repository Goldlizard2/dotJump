# File:   Makefile
# Author: Okoko Anainga, Daniel Pallesen
# Date:   19/10/2021
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../../utils -I../../../fonts -I../../../drivers -I../../../drivers/avr -std=c99
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: gameEnvironment.out


# Compile: create object files from C source files.
gameEnvironment.o: gameEnvironment.c ../../../drivers/avr/system.h game.h
	$(CC) -c $(CFLAGS) $< -o $@

game.o: game.c ../../../drivers/avr/system.h game.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../../drivers/avr/pio.c ../../../drivers/avr/pio.h ../../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../../drivers/avr/system.c ../../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../../drivers/avr/timer.c ../../../drivers/avr/system.h ../../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../../drivers/display.c ../../../drivers/avr/system.h ../../../drivers/display.h ../../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../../drivers/ledmat.c ../../../drivers/avr/pio.h ../../../drivers/avr/system.h ../../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../../utils/font.c ../../../drivers/avr/system.h ../../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../../utils/pacer.c ../../../drivers/avr/system.h ../../../drivers/avr/timer.h ../../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../../utils/tinygl.c ../../../drivers/avr/system.h ../../../drivers/display.h ../../../utils/font.h ../../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@
	
navswitch.o: ../../../drivers/navswitch.c ../../../drivers/avr/delay.h ../../../drivers/avr/pio.h ../../../drivers/avr/system.h ../../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@
	
button.o: ../../../drivers/button.c ../../../drivers/avr/pio.h ../../../drivers/avr/system.h ../../../drivers/button.h
	$(CC) -c $(CFLAGS) $< -o $@



# Link: create ELF output file from object files.
game.out: gameEnvironment.o game.o pio.o system.o timer.o display.o ledmat.o font.o pacer.o tinygl.o navswitch.o button.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@





# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex



# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


