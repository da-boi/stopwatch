#include <stdio.h>
#include <string.h>

#include "strutils.h"


#define ASCII_NUMBER_OFFSET 48


char *sinv(char *s) {
    int len = 0;
    int i = 0;
    char swap;

    len = strlen(s);
    for (i=0; i < len/2; i++) {
        swap = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = swap;
    }

    return s;
}

char *utos(char *s, unsigned int u) {
    int i = 0;

    do {
        s[i++] = (u % 10) + ASCII_NUMBER_OFFSET;
    } while ((u /= 10) != 0);
    s[i] = '\0';
    sinv(s);

    return s;
}

char *spad(char *dest, char *src, char pad, int n) {
    int dif = 0;

    dif = n - strlen(src);
    if (dif < 0) {
        strcpy(dest, src);
    } else {
        strcpy(&(dest[dif]), src);
        while (dif-- > 0) dest[dif] = pad;
    }

    return dest;
}
