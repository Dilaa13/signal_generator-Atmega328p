# Name of the program (no extension)
TARGET = main

# Source file
SRC = main.c

# MCU and clock
MCU = atmega328p
F_CPU = 16000000UL

# Compiler and flags
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -mmcu=atmega328p -DF_CPU=16000000UL -Os -std=c99
LDFLAGS = -mmcu=$(MCU)

# Output format
FORMAT = ihex

# Default target
all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(TARGET).o
	$(CC) $(LDFLAGS) $< -o $@

%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

clean:
	rm -f *.o *.elf *.hex
