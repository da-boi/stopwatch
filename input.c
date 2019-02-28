#include <avr/interrupt.h>
#include <avr/io.h>

#include "input.h"


void input_init(void) {
    SFIOR |=
          (1 << PUD)
    ;

    INPUT_DDR &= ~(
          (1 << START_P)
        | (1 << RESET_P)
        | (1 << STOP_P)
    );
}
