#include <avr/io.h>
#include <avr/interrupt.h>

#include "display.h"


int main(void) {
	display_init();
	sei();
	display_set("hello ");
	while (1) {


	}
	return 0;
}
