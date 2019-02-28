#ifndef STRUTILS_H
#define STRUTILS_H

#include <stdint.h>

#define ASCII_NUMBER_OFFSET 48


/*
 * Copies the string 'src' to 'dest' and pads to 'n' places with the char
 * given by ’pad’
 * Returns the pointer of 'dest'
 * The user must ensure that 'dest' is big enough
 */
char *spad(char *dest, char *src, char pad, int n);

/*
 * Inverts the given string
 * Returns the pointer to the string
 */
char *sinv(char *s);

/*
 * Converts the unsigned integer 'u' to a c-style string and stores it the
 * buffer 's'
 * The user must ensure that the buffer 's' is big enough to receive the string
 * Returns the pointer to 's'
 */
char *utos(char *s, uint64_t u);

#endif
