#ifndef TIME_H
#define TIME_H

#ifdef TIMER_2
#error "Timer2 already in use"
#else
#define TIMER_2
#endif

#ifndef F_CPU
#define F_CPU 2000000UL
#else
#if F_CPU != 2000000UL
#warning "F_CPU expected to be 2MHz - undefined behaviour"
#endif
#endif

typedef uint64_t micros_t;
typedef uint64_t millis_t;

/*
 * Sets up Timer2 for time keeping
 * sei() needs to be called afterwards
 */
void time_init(void);

/*
 * Returns time since init()
 *   in microseconds
 * Resolution: 4us
 * Overflows after ~584,942 years
 */
micros_t time_get_micros(void);

/*
 * Returns time since init()
 *   in milliseconds
 *  Resolution: 1.024ms
 *  Overflows after ~584,942,417 years
 * /
millis_t time_get_millis(void);

/* Returns after given microseconds */
void time_delay_micros(micros_t time);

/* Returns after given milliseconds */
void time_delay_micros(millis_t time);

#endif
