#define F_CPU 2000000UL
#define MAX_COUNT 100

#include <avr/interrupt.h>
#include <string.h>

#include "display.h"
#include "highscore.h"
#include "input.h"
#include "strutils.h"
#include "time.h"

int main(void) {
    enum {CLOCK, COUNTER} mode = CLOCK;
    enum {IDLE, STARTED, STOPPED} state = IDLE;
    enum {LEFT, RIGHT} side = LEFT;
    char tstring[32];
    Millis time = 0;
    Millis t0 = 0;
    unsigned int counter[2] = {0, 0};

    time_init();
    display_init();
    sei();
    input_init();

    display_set("hall0 ");
    time_delay(1000);

    while (START == PRESSED) {
        if ((time_get() - t0) > 1000) {
            display_set("------");
        }
        if ((time_get() - t0) > 3000) {
            mode = COUNTER;
            break;
        }
    }

    switch (mode) {

        /* ################################################################## */
        case CLOCK:

            while (1) {
                switch (state) {
                    
                    /* ------------------------------------------------------ */
                    case IDLE:

                        display_set(time_to_string(tstring, 0));
                        
                        if (START == PRESSED) {
                            state = STARTED;
                            time_reset();
                            t0 = time_get();
                        }

                        break;

                    /* ------------------------------------------------------ */
                    case STARTED:

                        display_set(time_to_string(tstring, time_get()/100));

                        if (START == PRESSED && time_get() - t0 > 600) {
                            state = STOPPED;
                            t0 = time_get();
                            display_set(time_to_string(tstring, t0/100));
                        }


                        break;

                    /* ------------------------------------------------------ */
                    case STOPPED:

                        if (RESET == PRESSED) {
                            state = IDLE;
                        }
                        else if (START == PRESSED && time_get() - t0 > 600) {
                            state = STARTED;
                            time_set(t0);
                            t0 = time_get();
                        }
                        break;

                    /* ------------------------------------------------------ */
                    default:

                        display_set("err 00");
                        while (1);

                        break;
                }
            }

        break;

        /* ################################################################## */
        case COUNTER:

            
            #define INC_COUNTER counter[side] = (counter[side] + 1) % MAX_COUNT;
            #define DEC_COUNTER counter[side] = (counter[side] - 1) % MAX_COUNT;

            display_set("00- 00");

            while (1) {

                if (START == PRESSED) {
                    side = (side + 1) % 2;
                    time_delay(DEBOUNCE_DELAY);
                }

                else if (UP == PRESSED) {
                    t0 = time_get();
                    INC_COUNTER; 
                    while (UP == PRESSED) {
                        if ((time_get() - t0) > 1000) {
                            INC_COUNTER;
                            time_delay(100);
                        }
                    }
                } 

                else if (DOWN == PRESSED) {
                    t0 = time_get();
                    DEC_COUNTER; 
                    while (UP == PRESSED) {
                        if ((time_get() - t0) > 1000) {
                            DEC_COUNTER;
                            time_delay(100);
                        }
                    }
                } 

            }

            break;

    }

    return 0;
}
