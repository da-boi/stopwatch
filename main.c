#define F_CPU 2000000UL

#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"
#include "input.h"
#include "strutils.h"
#include "time.h"


#define EEPROM_HIGHSCORE (void *) 0


char *time_to_string(char *buffer, Millis t);
Millis get_highscore(void);
void store_highscore(Millis highscore);

int main(void) {
    enum {IDLE, STARTED, STOPPED} state = IDLE;
    char tstring[32];
    char hstring[32];
    Millis highscore = 0;
    Millis time = 0;
    Millis t0 = 0;

    time_init();
    display_init();
    sei();
    input_init();

    highscore = get_highscore();
    if (highscore == 0) {
        strcpy(hstring, "h ----");
    } else {
        time_to_string(hstring, highscore/100);
        hstring[0] = 'h';
    }

    display_set("hello ");
    t0 = time_get_millis();
    while ((time_get_millis() - t0) < 1000);


    while (1) {
        switch (state) {
            
            /* -------------------------------------------------------------- */
            case IDLE:
                display_set(hstring);
                
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
                            highscore = 0;
                            strcpy(hstring, "h ----");
                            display_set(hstring);
                            break;
                        }
                    }
                    while (RESET == PRESSED);
                    
                }

                break;

            /* -------------------------------------------------------------- */
            case STARTED:
                display_set(time_to_string(tstring, time_get_millis()/100));

                if (STOP == PRESSED) {
                    state = STOPPED;
                    time = time_get_millis();
                    display_set(time_to_string(tstring, time/100));
                    if (highscore == 0
                     || time < highscore) {
                        store_highscore(time);
                        highscore = time;
                        time_to_string(hstring, highscore/100);
                        hstring[0] = 'h';
                    }
                }

                break;

            /* -------------------------------------------------------------- */
            case STOPPED:
                if (RESET == PRESSED) {
                    state = IDLE;
                    t0 = time_get_millis();
                    while ((time_get_millis() - t0) < 500);
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
    spad(buffer, tmp, '0', 2);
    strncpy(tmp, buffer, 32);
    spad(buffer, tmp, ' ', 6);

    return buffer;
}

Millis get_highscore(void) {
    Millis highscore = 0;

    eeprom_read_block(
            (void *) &highscore,
            EEPROM_HIGHSCORE,
            sizeof(Millis)
    );

    return highscore;
}

void store_highscore(Millis highscore) {
    eeprom_update_block(
            (void *) &highscore,
            EEPROM_HIGHSCORE,
            sizeof(Millis)
    );

    return;
}
