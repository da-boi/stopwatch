#include <avr/io.h>
#include <avr/interrupt.h>

#include "time.h"


uint64_t g_timer2OvfCount = 0;

void time_init(void) {
	TCCR2A =
		  (0b0000 << COM2B0)	// Disable all port operation
		| (0b00 << WGM20)	// Normal mode (also WGM2 in TCCR2B)
	;

	TCCR2B =
		  (0b0 << WGM22)		// See WGM0 in TCCR2A
		| (0b010 << CS20)	// Clk_io/8
	;

	TIMSK2 = (0b1 << TOIE2);	// Oveflow interrupt enable
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
