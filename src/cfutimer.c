/*
 * cfutimer.c - This file is part of the libcfu library
 *
 * Copyright (c) Matthew Brush <mbrush@codebrainz.ca>.
 * All rights reserved.
 *
 * This code is released under the BSD license, see COPYING file
 * for full license text.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "cfu.h"
#include "cfutimer.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

/* For Windows, use a fairly accurate timer. This could probably be
 * improved quite a bit (if it even works at all), by using performance
 * counters stuff or thread cycle time from the API. Setting the afinity
 * mask is probably also helpful. Note: not tested at all. */
#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# define TIMEVALUE_TYPE    FILETIME
# define TIMEVALUE_NOW(tv) GetSystemTimeAsFileTime(&(tv))
# define TIMEVALUE_SEC(tv) ((double)(((uint64_t)(tv).dwHighDataTime << 32) + (tv).dwLowDateTime) * 10000000.0)
/* another option for windows, also untested
#define TIMEVALUE_TYPE LARGE_INTEGER
// future: set thread afinity mask
#define TIMEVALUE_NOW(tv) QueryPerformanceCounter(&(tv))
static INLINE double timevalue_sec_(TIMEVALUE_TYPE *tv) {
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  return (double) tv->QuadPart / (double) freq.QuadPart;
}
#define TIMEVALUE_SEC(tv) timevalue_sec_(&(tv))
*/
#else
/* For non-Windows systems check to see if the nano-second timer is
 * available from POSIX. Note: must also be linked with -lrt.*/
# if defined(HAVE_CLOCK_GETTIME) && defined(CLOCK_MONOTONIC)
#  define TIMEVALUE_TYPE    struct timespec
#  define TIMEVALUE_NOW(tv) clock_gettime(CLOCK_MONOTONIC, &(tv))
#  define TIMEVALUE_SEC(tv) ((double)(tv).tv_sec + ((double) (tv).tv_nsec / 1000000000.0))
/* For non-Windows systems that don't have the more accurate clock_gettime()
 * then use the gettimeofday() function from POSIX if available. */
# elif defined(HAVE_GETTIMEOFDAY)
#  define TIMEVALUE_TYPE    struct timeval
#  define TIMEVALUE_NOW(tv) gettimeofday(&(tv), NULL)
#  define TIMEVALUE_SEC(tv) ((double)(tv).tv_sec + ((double) (tv).tv_usec / 1000000.0))
# else /* Fall-back to time() from stdlib */
#  define TIMEVALUE_TYPE    time_t
#  define TIMEVALUE_NOW(tv) time(&(tv))
#  define TIMEVALUE_SEC(tv) ((double)(tv))
# endif
#endif

struct cfutimer
{
	libcfu_type type;
	TIMEVALUE_TYPE t1;
	TIMEVALUE_TYPE t2;
};

cfutimer_t *cfutimer_new(void)
{
	cfutimer_t *timer = calloc(1, sizeof(cfutimer_t));
	timer->type = libcfu_t_time;
	return timer;
}

void cfutimer_start(cfutimer_t *timer)
{
	memset(&(timer->t2), 0, sizeof(TIMEVALUE_TYPE));
	TIMEVALUE_NOW(timer->t1);
}

void cfutimer_stop(cfutimer_t *timer)
{
	TIMEVALUE_NOW(timer->t2);
}

double cfutimer_elapsed(cfutimer_t *timer)
{
	return TIMEVALUE_SEC(timer->t2) - TIMEVALUE_SEC(timer->t1);
}

void cfutimer_free(cfutimer_t *timer)
{
	free(timer);
}
