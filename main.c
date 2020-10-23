#define F_CPU 2000000UL

#include <avr/interrupt.h>
#include <string.h>

#include "display.h"
#include "highscore.h"
#include "input.h"
#include "strutils.h"
#include "time.h"

#define MAX_COUNT 100
#define true 1
#define false 0
#define BOOL int

int main(void) {
    enum {CLOCK, COUNTER, SPRINTZ} mode = CLOCK;
    enum {IDLE, STARTED, STOPPED} state = IDLE;
    enum {LEFT, RIGHT} side = LEFT;
    char tstring[32];
    Millis time = 0;
    Millis t0 = 0;
    Millis t1 = 0;
    unsigned int counter[2] = {0, 0};
    BOOL wasReleased = false;
    BOOL aua = false;

    time_init();
    display_init();
    sei();
    input_init();

    display_set("budder");
    time_delay(1000);
    display_set("bei   ");
    time_delay(1000);
    display_set("die   ");
    time_delay(1000);
    display_set("fische");
    time_delay(1000);

    t0 = time_get();

    while (START == PRESSED) {
        if ((time_get() - t0) > 1000) {
            mode = COUNTER;
            break;
        }
    }

    while (RESET == PRESSED) {
        if ((time_get() - t0) > 1000) {
            mode = SPRINTZ;
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
                        
                        if (START == PRESSED && time_get() - t0 > DEBOUNCE_DELAY) {
                            state = STARTED;
                            time_reset();
                            t0 = time_get();
                        }
                        t1 = time_get();
                        while (DOWN == PRESSED) {
                            if ((time_get() - t1) > 800) {
                                aua = true;
                                break;
                            }
                        }

                        break;

                    /* ------------------------------------------------------ */
                    case STARTED:

                        display_set(time_to_string(tstring, time_get()/10));

                        if (START == PRESSED
                            && time_get() - t0 > DEBOUNCE_DELAY
                            && wasReleased == true) {
                            wasReleased = false;
                            state = STOPPED;
                            t0 = time_get();
                            if (aua == true){
                                display_set(" AUA  ");
                                time_delay(500);
                                aua = false;
                            }
                            display_set(time_to_string(tstring, t0/10));
                        }
                        if (START != PRESSED) wasReleased = true;


                        break;

                    /* ------------------------------------------------------ */
                    case STOPPED:

                        if (RESET == PRESSED) {
                            state = IDLE;
                        }
                        else if (START == PRESSED
                                 && time_get() - t0 > DEBOUNCE_DELAY
                                 && wasReleased == true) {
                            wasReleased = false;
                            state = STARTED;
                            time_set(t0);
                            t0 = time_get();
                        }
                        if (START != PRESSED) wasReleased = true;
                        t1 = time_get(); 
                        while (DOWN == PRESSED) {
                            if ((time_get() - t1) > 800) {
                                aua = true;
                                break;
                            }
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

        /* ################################################################## */
        case SPRINTZ:

            while (1) {
                switch (state) {
                    
                    /* ------------------------------------------------------ */
                    case IDLE:

                        display_set(" SAUF ");
                        
                        if (START == PRESSED && time_get() - t0 > DEBOUNCE_DELAY) {
                            state = STARTED;
                            t0 = time_get();
                            display_set("  33  ");
                            time_delay(1000);
                            display_set("  22  ");
                            time_delay(1000);
                            display_set("  11  ");
                            time_delay(1000);
                            time = time_get();
                        }

                        break;

                    /* ------------------------------------------------------ */
                    case STARTED:

                        display_set(time_to_string(tstring, (time_get()-time)/10));

                        if (START == PRESSED && time_get() - t0 > DEBOUNCE_DELAY) {
                            state = STOPPED;
                            t0 = time_get();
                            display_set(time_to_string(tstring, (t0-time)/10));
                        }

                        break;

                    /* ------------------------------------------------------ */
                    case STOPPED:

                        if (RESET == PRESSED) {
                            state = IDLE;
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

    }

    return 0;
}
