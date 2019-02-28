#define F_CPU 2000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "strutils.h"
#include "time.h"


char *time_to_string(char *buffer, Millis t);

int main(void) {
    char tstring[32];

    time_init();
    display_init();
	sei();

    while (1) {
        display_set(time_to_string(tstring, time_get_millis()/10));
    }

    return 0;
}

char *time_to_string(char *buffer, Millis t) {
    char tmp[32];
    
    utos(tmp, (uint64_t) t);
    spad(buffer, tmp, ' ', 6);

    return buffer;
}
