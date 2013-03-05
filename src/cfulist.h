/*
 * cfulist.h - This file is part of the libcfu library
 *
 * Copyright (c) 2005 Don Owens. All rights reserved.
 *
 * This code is released under the BSD license:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   * Neither the name of the author nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CFU_LIST_
#define CFU_LIST_

#include <cfu.h>
#include <stddef.h>

CFU_BEGIN_DECLS

/* The list itself. */
typedef struct cfulist cfulist_t;

/* Function called for each element in the list when passed to
 * cfulist_foreach().  A non-zero return value means to stop
 * iteration.
 */
typedef int (*cfulist_foreach_fn_t)(void *data, size_t data_size, void *arg);

/* Function called for each element in the list when passed to
 * cfulist_map().  The return value is used to build a new
 * list.
 */
typedef void * (*cfulist_map_fn_t)(void *data, size_t data_size, void *arg,
	size_t *new_data_size);

/* Function called to free the data in an element. */
typedef void (*cfulist_free_fn_t)(void *data);

/* Returns a new list. */
cfulist_t * cfulist_new(void);

/* Same as cfulist_new(), but set a function to be called on each
 * element when the list is destroyed.
 */
cfulist_t * cfulist_new_with_free_fn(cfulist_free_fn_t free_fn);

/* Returns the number of entries in the list. */
size_t cfulist_num_entries(cfulist_t *list);

/* Manipulating list entries */

/* Push a value onto the end of the list. */
int cfulist_push_data(cfulist_t *list, void *data, size_t data_size);

/* Pop a value from the end of the list. */
int cfulist_pop_data(cfulist_t *list, void **data, size_t *data_size);

/* Add a value at the beginning of the list. */
int cfulist_unshift_data(cfulist_t *list, void *data, size_t data_size);

/* Shift a value off the beginning of the list. */
int cfulist_shift_data(cfulist_t *list, void **data, size_t *data_size);

/* Add a value at the end of the queue (equivalent to push) */
int cfulist_enqueue_data(cfulist_t *list, void *data, size_t data_size);

/* Remove the value at the beginning of the list (equivalent to shift). */
int cfulist_dequeue_data(cfulist_t *list, void **data, size_t *data_size);

/* Return the last entry from the list (without removing it from
 * the list).
 */
int cfulist_first_data(cfulist_t *list, void **data, size_t *data_size);

/* Return the last entry from the list (without removing it from
 * the list).
 */
int cfulist_last_data(cfulist_t *list, void **data, size_t *data_size);

/* Return the nth entry from the list (without removing it from
 * the list).  n starts at zero.
*/
int cfulist_nth_data(cfulist_t *list, void **data, size_t *data_size, size_t n);

void cfulist_reset_each(cfulist_t *list);
int cfulist_each_data(cfulist_t *list, void **data, size_t *data_size);
int cfulist_next_data(cfulist_t *list, void **data, size_t *data_size);

/* Calls fe_fn() for each element in the list. Also passes arg on
 * each call. If fe_fn() returns a non-zero value, the iteration
 * over the elements stops.
 */
size_t cfulist_foreach(cfulist_t *list, cfulist_foreach_fn_t fe_fn, void *arg);

/* Creates a new list from the list passed in.  Calls map_fn() on
 * each element in the list.  The return value is placed in the
 * corresponding position in the new list.
 */
cfulist_t *cfulist_map(cfulist_t *list, cfulist_map_fn_t map_fn, void *arg);

/* Free all resources used by the list. */
void cfulist_destroy(cfulist_t *list);
void cfulist_destroy_with_free_fn(cfulist_t *list, cfulist_free_fn_t free_fn);

/* When you don't care about the size of the data */

int cfulist_push(cfulist_t *list, void *data);
void * cfulist_pop(cfulist_t *list);
int cfulist_unshift(cfulist_t *list, void *data);
void * cfulist_shift(cfulist_t *list);
int cfulist_enqueue(cfulist_t *list, void *data);
void *cfulist_dequeue(cfulist_t *list);

/* Strings -- assume data is a null-terminated string -- size is
 * calculated by strlen(data) + 1
 */

int cfulist_push_string(cfulist_t *list, char *data);
char * cfulist_pop_string(cfulist_t *list);
int cfulist_unshift_string(cfulist_t *list, char *data);
char * cfulist_shift_string(cfulist_t *list);
int cfulist_enqueue_string(cfulist_t *list, char *data);
char *cfulist_dequeue_string(cfulist_t *list);

char *cfulist_join(cfulist_t *list, const char *delimiter);

CFU_END_DECLS

#endif
