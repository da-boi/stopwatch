#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "strutils.h"
#include "time.h"


uint64_t g_timer2OvfCount = 0;

void time_init(void) {
    TCCR2 =
          (0b00     << COM20)   // Disable all port operation
        | (0b0      << WGM20)   // Normal mode (also WGM21)
        | (0b0      << WGM21)
        | (0b100    << CS20)    // clk_T2S/64
    ;

    TIMSK |= (0b1 << TOIE2);    // Overflow interrupt enable
}

void time_reset(void) {
    g_timer2OvfCount = 0;
    TCNT2 = 0;
}

Millis time_get(void) {
    return g_timer2OvfCount * 1024 / 1000;
}

void time_set(Millis t) {
    g_timer2OvfCount = t * 1000 / 1024;
}

void time_delay(Millis t) {
    Millis t0 = 0;

    t0 = time_get();
    while ((time_get() - t0) < t);
}

char *time_to_string(char *buffer, Millis t) {
    char tmp[32];
    
    utos(tmp, (uint64_t) t);
    spad(buffer, tmp, '0', 2);
    strncpy(tmp, buffer, 32);
    spad(buffer, tmp, ' ', 6);

    return buffer;
}

ISR(TIMER2_OVF_vect) {
    g_timer2OvfCount++;
}
