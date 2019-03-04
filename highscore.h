#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include "time.h"

#define EEPROM_HIGHSCORE (void *) 0


/*
 * Returns the highscore stored in EEPROM
 */
Millis fetch_highscore(void);

/*
 * Stores the highscore into EEPROM
 */
void store_highscore(Millis highscore);

#endif
