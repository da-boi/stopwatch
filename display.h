#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef TIMER_0
#error "Timer0 already used"
#else
#define TIMER_0
#endif

#define NUMBER_OF_DIGITS 6

#define SEG_PORT    PORTD
#define SEG_DDR     DDRD
#define DIGIT_PORT  PORTC
#define DIGIT_DDR   DDRC

/* Digit pins */
#define DIGIT_0 PC0
#define DIGIT_1 PC1
#define DIGIT_2 PC2
#define DIGIT_3 PC3
#define DIGIT_4 PC4
#define DIGIT_5 PC5

/* Segment pins */
#define SEG_A PD0
#define SEG_B PD1
#define SEG_C PD2
#define SEG_D PD3
#define SEG_E PD4
#define SEG_F PD5
#define SEG_G PD6

/*
 * this function needs to be called for the display to work
 */
void display_init(void);

void display_set(char *);

void _display_set_segments(char c);

#endif
