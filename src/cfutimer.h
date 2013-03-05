/*
 * cfutimer.h - This file is part of the libcfu library
 *
 * Copyright (c) Matthew Brush <mbrush@codebrainz.ca>.
 * All rights reserved.
 *
 * This code is released under the BSD license, see COPYING file
 * for full license text.
 */

#ifndef CFU_TIMER_H_
#define CFU_TIMER_H_

#include <cfu.h>

CFU_BEGIN_DECLS

typedef struct cfutimer cfutimer_t;

/* Return a new cfutimer structure. */
cfutimer_t *cfutimer_new(void);

/* Start the timer. */
void cfutimer_start(cfutimer_t *timer);

/* Stop the timer. */
void cfutimer_stop(cfutimer_t *timer);

/* Return the number of seconds elapsed as a double. */
double cfutimer_elapsed(cfutimer_t *timer);

/* Deallocate resources allocated for time. */
void cfutimer_free(cfutimer_t *timer);

CFU_END_DECLS

#endif /* CFU_TIMER_H_ */
