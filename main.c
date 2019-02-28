#define F_CPU 2000000UL

#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "strutils.h"
#include "time.h"


#define EEPROM_HIGHSCORE = (const void *) 0;


char *time_to_string(char *buffer, Millis t);
Millis get_highscore(void);
void store_highscore(Millis highscore);

int main(void) {
    enum {IDLE, STARTED, STOPPED} state = IDLE;
    char tstring[32];
    Millis highscore = 0;
    Millis time = 0;
    Millis t0 = 0;

    time_init();
    display_init();
	sei();
    input_init();

    highscore = get_highscore();

    while (1) {
        switch (state) {
            
            /* -------------------------------------------------------------- */
            case IDLE:
                display_set(time_to_string(tstring, highscore);
                
                if (START == PRESSED) {
                    state = STARTED;
                    time_reset();
                }
                else if (RESET == PRESSED) {
                    display_set("------");
                    t0 = time_get_millis();
                    while (RESET == PRESSED) {
                        if ((time_get_millis() - t0) > 3000) {
                            store_highscore(0);
                        }
                    }
                }

                break;

            /* -------------------------------------------------------------- */
            case STARTED:
                display_set(time_to_string(tstring, time_get_millis()));

                if (STOP == PRESSED) {
                    state = STOPPED;
                    time = time_get_millis();
                    if (time < highscore) {
                        store_highscore(time);
                    }
                }

                break;

            /* -------------------------------------------------------------- */
            case STOPPED:
                display_set(time_to_string(tstring, time));

                if (RESET == PRESSED) {
                    state = IDLE;
                }

                break;

            /* -------------------------------------------------------------- */
            default:
                display_set("err 00");
                while (1);
                break;
        }
    }

    return 0;
}

char *time_to_string(char *buffer, Millis t) {
    char tmp[32];
    
    utos(tmp, (uint64_t) t);
    spad(buffer, tmp, ' ', 6);

    return buffer;
}

Millis get_highscore(void) {
    Millis highscore = 0;

    eeprom_read_block(
            (void *) highscore,
            EEPROM_HIGHSCORE,
            sizeof(Millis)
    );

    return highscore;
}

void store_highscore(Millis highscore) {
    eeprom_update_block(
            (void *) &((Millis) 0),
            EEPROM_HIGHSCORE,
            sizeof(Millis)
    );

    return;
}
