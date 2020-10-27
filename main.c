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

void display_counter(int l, int r, int side);
int mod(int a, int b);
enum {LEFT, RIGHT} side = LEFT;

int main(void) {
    enum {CLOCK, COUNTER, SPRINTZ} mode = CLOCK;
    enum {IDLE, STARTED, STOPPED} state = IDLE;
    char tstring[32];
    Millis time = 0;
    Millis t0 = 0;
    Millis t1 = 0;
    int counter[2] = {0, 0};
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

                
            #define INC_COUNTER counter[side] = mod((counter[side] + 1), MAX_COUNT);
            #define DEC_COUNTER counter[side] = mod((counter[side] - 1), MAX_COUNT);

            display_set("00- 00");

            while(START == PRESSED);

            while (1) {

                if (START == PRESSED
                    && time_get() - t0 > DEBOUNCE_DELAY
                    && wasReleased == true) {
                    wasReleased = false;
                    t0 = time_get();
                    side = (side + 1) % 2;
                    display_counter(counter[LEFT], counter[RIGHT], side);
                }

                if (START != PRESSED) wasReleased = true;

                if (UP == PRESSED) {
                    t0 = time_get();
                    INC_COUNTER; 
                    display_counter(counter[LEFT], counter[RIGHT], side);
                    while (UP == PRESSED) {
                        if ((time_get() - t0) > 1000) {
                            INC_COUNTER;
                            display_counter(counter[LEFT], counter[RIGHT], side);
                            time_delay(67);
                        }
                    }
                } 

                if (DOWN == PRESSED) {
                    t0 = time_get();
                    DEC_COUNTER; 
                    display_counter(counter[LEFT], counter[RIGHT], side);
                    while (DOWN == PRESSED) {
                        if ((time_get() - t0) > 1000) {
                            DEC_COUNTER;
                            display_counter(counter[LEFT], counter[RIGHT], side);
                            time_delay(67);
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
                            t0 = time_get();
                            display_set("HALTEN");
                            while (START == PRESSED) {
                                if ((time_get() - t0) > 1000) {
                                    state = STARTED;
                                    display_set(" LOS  ");
                                    break;
                                }
                            }
                            while (START == PRESSED);
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


void display_counter(int l, int r, int side){
        char left[3];
        char right[3];
        char pleft[3];
        char pright[3];
        char str[7];

        utos(left, (uint64_t) l);
        utos(right, (uint64_t) r);
        spad(pleft, left, '0', 2);
        spad(pright, right, '0', 2);
        strcpy(str, pleft);
        if (side == LEFT)   strcat(str, "- ");
        else                strcat(str, " -");
        strcat(str, pright) ;

        display_set(str);
}

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}
