#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#define SINE_POINTS 64
#define PI 3.14159265

uint16_t sine_table[SINE_POINTS];

// Function to initialize SPI
void SPI_init() {
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2); // MOSI, SCK, SS
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Enable SPI, Master, f/16
}

// Function to send data to MCP4921
void MCP4921_send(uint16_t data) {
    uint8_t high_byte = (data >> 8) & 0x0F;
    high_byte |= 0x30; // 0b00110000: config bits for DAC A, buffered, gain=1x, active

    PORTB &= ~(1 << PB2);         // SS low
    SPDR = high_byte;
    while (!(SPSR & (1 << SPIF)));
    SPDR = data & 0xFF;
    while (!(SPSR & (1 << SPIF)));
    PORTB |= (1 << PB2);          // SS high
}

// Generate sine values in the lookup table
void generate_sine_table() {
    for (uint8_t i = 0; i < SINE_POINTS; i++) {
        float angle = (2 * PI * i) / SINE_POINTS;
        sine_table[i] = (uint16_t)(2047 + 2047 * sin(angle)); // Scale to 0–4095
    }
}

int main(void) {
    SPI_init();
    generate_sine_table();

    while (1) {
        for (uint8_t i = 0; i < SINE_POINTS; i++) {
            MCP4921_send(sine_table[i]);
            _delay_us(200); // adjust for frequency
        }
    }
}