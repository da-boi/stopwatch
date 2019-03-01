#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "display.h"

void display_set_segments(char c);

typedef uint8_t dbuffer_t[NUMBER_OF_DIGITS];

const char
    *s_err          = "err   ",
    *s_hello        = "hello ",
    *s_overflow     = "of    ",
    *s_highscore    = "hi    "
;

dbuffer_t g_dbuffer = {
    '0',
    '0',
    '0',
    '0',
    '0',
    '0'
};

uint8_t g_digitPin[] = {
    DIGIT_0,
    DIGIT_1,
    DIGIT_2,
    DIGIT_3,
    DIGIT_4,
    DIGIT_5
};

uint8_t g_digitIndex = 0;


void display_init(void) {
    /*
     * Sets up Timer/Counter0 as input
     *   for the multiplexer
     */
    
    TCCR0 =
          (0b010    << CS00)    // clk_io/8
    ;

    TIMSK |=
          (0b1      << TOIE0)   // Enable overflow interrupt
    ;
    
    /* Sets all display necessary pins to output */
    DDRC = 0xFF;
    DDRD = 0xFF;    
}

void display_set(char *str) {
    for (int i=0; i < NUMBER_OF_DIGITS; i++) {
        g_dbuffer[i] = str[NUMBER_OF_DIGITS-1 -i];
    }
}

void display_set_segments(char c) {
    uint8_t segPort = 0x00;

    switch (c) {
        case '-': segPort = 0x40; break;
        case '0': segPort = 0x3F; break;
        case '1': segPort = 0x06; break;
        case '2': segPort = 0x5B; break;
        case '3': segPort = 0x4F; break;
        case '4': segPort = 0x66; break;
        case '5': segPort = 0x6D; break;
        case '6': segPort = 0x7D; break;
        case '7': segPort = 0x07; break;
        case '8': segPort = 0x7F; break;
        case '9': segPort = 0x6F; break;
        case ' ': segPort = 0x00; break;
        case 'e': segPort = 0x79; break;
        case 'h': segPort = 0x76; break;
        case 'i': segPort = 0x30; break;
        case 'l': segPort = 0x38; break;
        case 'o': segPort = 0x5C; break;
        case 'r': segPort = 0x50; break;
        default: segPort = 0x08; break; // '_'
    }

    PORTD = segPort;
}


ISR(TIMER0_OVF_vect) {
    /* Turn off all digits */
    PORTC = 0x00;

    /* Next digit */
    g_digitIndex++;
    if ( g_digitIndex == NUMBER_OF_DIGITS ) {
        g_digitIndex = 0;
    }

    /* Set the segments for the next digit */
    display_set_segments(g_dbuffer[g_digitIndex]);

    /* Turn on the next digit */
    PORTC |= (0b1 << g_digitPin[g_digitIndex]);
}
