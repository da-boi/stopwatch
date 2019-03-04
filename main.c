#define F_CPU 2000000UL

#include <avr/interrupt.h>
#include <string.h>

#include "display.h"
#include "highscore.h"
#include "input.h"
#include "time.h"


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

    highscore = fetch_highscore();
    if (highscore == 0) {
        strcpy(hstring, "h ----");
    } else {
        time_to_string(hstring, highscore/100);
        hstring[0] = 'h';
    }

    display_set("hello ");
    time_delay(1000);

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
                    t0 = time_get();
                    while (RESET == PRESSED) {
                        if ((time_get() - t0) > 1000) {
                            display_set("------");
                        }
                        if ((time_get() - t0) > 3000) {
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

                display_set(time_to_string(tstring, time_get()/100));

                if (STOP == PRESSED) {
                    state = STOPPED;
                    time = time_get();
                    display_set(time_to_string(tstring, time/100));
                }

                break;

            /* -------------------------------------------------------------- */
            case STOPPED:

                if (RESET == PRESSED) {
                    state = IDLE;
                    if (highscore == 0
                     || time < highscore) {
                        store_highscore(time);
                        highscore = time;
                        time_to_string(hstring, highscore/100);
                        hstring[0] = 'h';
                    }

                }
                else if (START == PRESSED) {
                    state = STARTED;
                    time_set(time);
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
