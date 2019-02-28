#include <avr/io.h>
#include <avr/interrupt.h>

#include "time.h"


uint64_t g_timer2OvfCount = 0;

void time_init(void) {
    TCCR2 =
          (0b00 << COM20)       // Disable all port operation
        | (0b0 << WGM20)        // Normal mode (also WGM21)
        | (0b0 << WGM21)
        | (0b010 << CS20)       // clk_T2S/8
    ;

    TIMSK |= (0b1 << TOIE2);    // Overflow interrupt enable
}

Micros time_get_micros(void) {
	return ((g_timer2OvfCount << 8) + TCNT2) * 4;
}

Millis time_get_millis(void) {
	return g_timer2OvfCount * 1024 / 1000;
}

ISR(TIMER2_OVF_vect) {
	g_timer2OvfCount++;
}
