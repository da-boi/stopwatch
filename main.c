#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "time.h"
#include "display.h"

char *time_to_string(char *buffer, Millis t);

int main(void) {
    time_init();
	display_init();
	sei();

    display_set(s_hello);

    char s[7];

    Millis t = 0;

	while (1) {
        // display_set(time_to_string(s, time_get_millis()));
	}
	return 0;
}

char *time_to_string(char *buffer, Millis t) {
    if (sprintf(buffer, "%06u", (unsigned int)t) < 0) {
        buffer = s_err;
    }
    return buffer;
}
