#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "time.h"
#include "display.h"


int main(void) {
	display_init();
	sei();

    display_set(s_err);

	while (1) {

	}
	return 0;
}
