#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "display.h"


char G_dbuffer[] = {
    '0',
    '0',
    '0',
    '0',
    '0',
    '0'
};

uint8_t G_digitPin[] = {
    DIGIT_0,
    DIGIT_1,
    DIGIT_2,
    DIGIT_3,
    DIGIT_4,
    DIGIT_5
};

uint8_t G_digitIndex = 0;


void display_init(void) {
    /*
     * Sets up Timer/Counter0 as input
     *   for the multiplexer
     */
    
    TCCR0 =
          (0b011    << CS00)    // 
    ;

    TIMSK |=
          (0b1      << TOIE0)   // Enables overflow interrupt
    ;
    
    /* Sets all display necessary pins to output */
    DIGIT_DDR   = 0xFF;
    SEG_DDR     = 0xFF;    
}

void display_set(char *str) {
    for (int i=0; i < NUMBER_OF_DIGITS; i++) {
        G_dbuffer[i] = str[NUMBER_OF_DIGITS-1 -i];
    }
}   

void _display_set_segments(char c) {
    switch (c) {
        case ' ': SEG_PORT = 0x00; break;
        case '-': SEG_PORT = 0x40; break;
        case '0': SEG_PORT = 0x3F; break;
        case '1': SEG_PORT = 0x06; break;
        case '2': SEG_PORT = 0x5B; break;
        case '3': SEG_PORT = 0x4F; break;
        case '4': SEG_PORT = 0x66; break;
        case '5': SEG_PORT = 0x6D; break;
        case '6': SEG_PORT = 0x7D; break;
        case '7': SEG_PORT = 0x07; break;
        case '8': SEG_PORT = 0x7F; break;
        case '9': SEG_PORT = 0x6F; break;
        case 'e': SEG_PORT = 0x79; break;
        case 'a': SEG_PORT = 0x77; break;
        case 'h': SEG_PORT = 0x76; break;
        case 'i': SEG_PORT = 0x30; break;
        case 'l': SEG_PORT = 0x38; break;
        case 'o': SEG_PORT = 0x5C; break;
        case 'r': SEG_PORT = 0x50; break;
        default:  SEG_PORT = 0x08; break; // '_'
    }
}


ISR(TIMER0_OVF_vect) {
    /* Turn off all digits */
    DIGIT_PORT = 0x00;

    /* Set the segments for the next digit */
    G_digitIndex = (G_digitIndex + 1) % NUMBER_OF_DIGITS;
    _display_set_segments(G_dbuffer[G_digitIndex]);

    /* Turn on the next digit */
    DIGIT_PORT |= (0b1 << G_digitPin[G_digitIndex]);
}
