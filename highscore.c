#include <avr/eeprom.h>

#include "highscore.h"


Millis fetch_highscore(void) {
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
