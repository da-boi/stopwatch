#ifndef INPUT_H
#define INPUT_H

#include <avr/io.h>

#define INPUT_PIN   PINB
#define INPUT_PORT  PORTB
#define INPUT_DDR   DDRB
#define START_P     PB0
#define RESET_P     PB1
#define STOP_P      PB2

#define START   (INPUT_PIN & (1 << START_P))
#define RESET   (INPUT_PIN & (1 << RESET_P))
#define STOP    (INPUT_PIN & (1 << STOP_P))
#define UP      RESET
#define DOWN    STOP

#define PRESSED 0

#define DEBOUNCE_DELAY 600
#define DEBOUNCED time_get() - t0 > DEBOUNCE_DELAY

void input_init(void);

#endif /* INPUT_H */
