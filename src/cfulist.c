/*
 * cfulist.c - This file is part of the libcfu library
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "cfu.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef HAVE_PTHREAD_H
# include <pthread.h>
#endif

#include "cfulist.h"
#include "cfustring.h"

typedef struct cfulist_entry {
	void *data;
	size_t data_size;
	struct cfulist_entry *next;
	struct cfulist_entry *prev;
} cfulist_entry;

struct cfulist {
	libcfu_type type;
	cfulist_entry *entries;
	cfulist_entry *tail;
	size_t num_entries;
#ifdef HAVE_PTHREAD_H
	pthread_mutex_t mutex;
#endif
	cfulist_entry *each_ptr;
	cfulist_free_fn_t free_fn;
};

cfulist_t *
cfulist_new(void) {
	cfulist_t *list = calloc(1, sizeof(cfulist_t));
#ifdef HAVE_PTHREAD_H
	pthread_mutex_init(&list->mutex, NULL);
#endif
	list->type = libcfu_t_list;
	return list;
}

cfulist_t *
cfulist_new_with_free_fn(cfulist_free_fn_t free_fn) {
	cfulist_t *list = cfulist_new();
	cfulist_set_free_function (list, free_fn);
	return list;
}

int
cfulist_set_free_function(cfulist_t *list, cfulist_free_fn_t ff) {
	if (ff) list->free_fn = ff;
	return 0;
}

size_t
cfulist_num_entries(cfulist_t *list) {
	return list->num_entries;
}

static CFU_INLINE void
lock_list(cfulist_t *list) {
#ifdef HAVE_PTHREAD_H
	pthread_mutex_lock(&list->mutex);
#endif
}

static CFU_INLINE void
unlock_list(cfulist_t *list) {
#ifdef HAVE_PTHREAD_H
	pthread_mutex_unlock(&list->mutex);
#endif
}

static CFU_INLINE cfulist_entry *
new_list_entry(void) {
	return calloc(1, sizeof(cfulist_entry));
}

int
cfulist_push_data(cfulist_t *list, void *data, size_t data_size) {
	cfulist_entry *entry = new_list_entry();
	if (!entry) return 0;

	if (data_size == (size_t)-1) data_size = strlen((char *)data) + 1;

	entry->data = data;
	entry->data_size = data_size;

	lock_list(list);

	if (list->tail) {
		entry->prev = list->tail;
		list->tail->next = entry;
		list->tail = entry;
	} else {
		list->tail = list->entries = entry;
	}
	list->num_entries++;

	unlock_list(list);

	return 1;
}

int
cfulist_pop_data(cfulist_t *list, void **data, size_t *data_size) {
	if (!list) {
		*data = NULL;
		*data_size = 0;
		return 0;
	}
	lock_list(list);

	if (list->tail) {
		if (list->tail->prev) {
			cfulist_entry *new_tail = list->tail->prev;
			assert(list->num_entries > 1);
			new_tail->next = NULL;
			*data = list->tail->data;
			if (data_size) *data_size = list->tail->data_size;
			free(list->tail);
			list->tail = new_tail;
		} else {
			/* there is only one entry in the list */
			assert(list->num_entries == 1);
			*data = list->tail->data;
			if (data_size) *data_size = list->tail->data_size;
			free(list->tail);
			list->tail = NULL;
			list->entries = NULL;
		}
		list->num_entries--;
		unlock_list(list);
		return 1;
	}

	unlock_list(list);

	if (data_size) *data_size = 0;
	return 0;
}

int
cfulist_unshift_data(cfulist_t *list, void *data, size_t data_size) {
	cfulist_entry *entry = new_list_entry();
	if (!entry) return 0;

	if (data_size == (size_t)-1) data_size = strlen((char *)data) + 1;

	entry->data = data;
	entry->data_size = data_size;

	lock_list(list);

	if (list->entries) {
		entry->next = list->entries;
		list->entries->prev = entry;
		list->entries = entry;
	} else {
		list->tail = list->entries = entry;
	}
	list->num_entries++;

	unlock_list(list);

	return 1;
}

int
cfulist_shift_data(cfulist_t *list, void **data, size_t *data_size) {
	int rv = 0;
	if (!list) {
		if (data_size) *data_size = 0;
		*data = NULL;
		return rv;
	}

	lock_list(list);

	if (list->entries) {
		cfulist_entry *entry = list->entries;
		assert(list->num_entries >= 1);
		rv = 1;
		*data = entry->data;
		if (data_size) *data_size = entry->data_size;
		if (entry->next) {
			assert(list->num_entries > 1);
			list->entries = entry->next;
			list->entries->prev = NULL;
		} else {
			assert(list->num_entries == 1);
			list->tail = NULL;
			list->entries = NULL;
		}
		free(entry);
		list->num_entries--;
	} else {
		assert(list->num_entries == 0);
		rv = 0;
		if (data_size) *data_size = 0;
		*data = NULL;
	}

	unlock_list(list);
	return rv;
}

int
cfulist_enqueue_data(cfulist_t *list, void *data, size_t data_size) {
	return cfulist_push_data(list, data, data_size);
}


int
cfulist_dequeue_data(cfulist_t *list, void **data, size_t *data_size) {
	return cfulist_shift_data(list, data, data_size);
}

int
cfulist_first_data(cfulist_t *list, void **data, size_t *data_size) {
	int rv = 0;
	if (!list) {
		return 0;
	}

	lock_list(list);
	if (list->entries) {
		rv = 1;
		*data = list->entries->data;
		if (data_size) *data_size = list->entries->data_size;
	} else {
		rv = 0;
		*data = NULL;
		*data_size = 0;
	}
	unlock_list(list);

	return rv;
}

int
cfulist_last_data(cfulist_t *list, void **data, size_t *data_size) {
	int rv = 0;
	if (!list) {
		return 0;
	}

	lock_list(list);
	if (list->tail) {
		rv = 1;
		*data = list->tail->data;
		if (data_size) *data_size = list->tail->data_size;
	} else {
		rv = 0;
		*data = NULL;
		*data_size = 0;
	}
	unlock_list(list);

	return rv;
}

int
cfulist_nth_data(cfulist_t *list, void **data, size_t *data_size, size_t n) {
	int rv = 0;
	size_t i = 0;
	cfulist_entry *ptr = NULL;

	if (!list) {
		return 0;
	}

	lock_list(list);
	if (list->entries) {
		for (i = 0, ptr = list->entries; ptr && i < n; i++, ptr = ptr->next);
		if (ptr && i == n) {
			rv = 1;
			*data = ptr->data;
			if (data_size) *data_size = list->entries->data_size;
		}
	} else {
		rv = 0;
		*data = NULL;
		*data_size = 0;
	}
	unlock_list(list);

	return rv;
}

void
cfulist_reset_each(cfulist_t *list) {
	if (!list) return;
	list->each_ptr = list->entries;
}

int
cfulist_each_data(cfulist_t *list, void **data, size_t *data_size) {
	if (!list) return 0;
	cfulist_reset_each(list);
	return cfulist_next_data(list, data, data_size);
}

int
cfulist_next_data(cfulist_t *list, void **data, size_t *data_size) {
	if (!list) return 0;
	*data = NULL;
	if (list->each_ptr) {
		*data = list->each_ptr->data;
		*data_size = list->each_ptr->data_size;
		list->each_ptr = list->each_ptr->next;
		return 1;
	}
	return 0;
}

size_t
cfulist_foreach(cfulist_t *list, cfulist_foreach_fn_t fe_fn, void *arg) {
	cfulist_entry *entry = NULL;
	size_t num_processed = 0;
	int rv = 0;

	if (!list) return 0;

	lock_list(list);
	for (entry = list->entries; entry && !rv; entry = entry->next) {
		rv = fe_fn(entry->data, entry->data_size, arg);
		num_processed++;
	}

	unlock_list(list);

	return num_processed;
}

typedef struct _cfulist_map_struct {
	cfulist_t *list;
	void *arg;
	cfulist_map_fn_t map_fn;
} _cfulist_map_ds;

static int
_cfulist_map_foreach(void *data, size_t data_size, void *arg) {
	_cfulist_map_ds *ds = (_cfulist_map_ds *)arg;
	size_t new_data_size = 0;
	void *rv = ds->map_fn(data, data_size, ds->arg, &new_data_size);
	cfulist_push_data(ds->list, rv, new_data_size);
	return 0;
}

cfulist_t *
cfulist_map(cfulist_t *list, cfulist_map_fn_t map_fn, void *arg) {
	cfulist_t *new_list = cfulist_new();
	_cfulist_map_ds ds;
	ds.list = new_list;
	ds.arg = arg;
	ds.map_fn = map_fn;

	cfulist_foreach(list, _cfulist_map_foreach, (void *)&ds);
	return new_list;
}

/* For when you don't care about the data size */

int
cfulist_push(cfulist_t *list, void *data) {
	return cfulist_push_data(list, data, 0);
}

void *
cfulist_pop(cfulist_t *list) {
	void *data = NULL;
	if (cfulist_pop_data(list, &data, NULL)) {
		return data;
	}
	return NULL;
}

void
cfulist_delete_data(cfulist_t *list, void *data) {
	cfulist_delete_data_with_free_fn(list, data, list->free_fn);
	return;
}

void
cfulist_delete_data_with_free_fn(cfulist_t *list, void *data, cfulist_free_fn_t free_fn) {
	if (!list) {
		return;
	}

	lock_list(list);

	cfulist_entry *entry = list->entries;
	while (entry)
	{ 
		if (entry->data == data) {
			if (!entry->prev) {
				if (entry->next) {
					assert(list->num_entries > 1);
					list->entries = entry->next;
					list->entries->prev = NULL;
				} else {
					assert(list->num_entries == 1);
					list->tail = NULL;
					list->entries = NULL;
				}
			} else {
				(entry->prev)->next = entry->next;
			}
			if (free_fn) free_fn(entry->data);
			free (entry);
			--list->num_entries;
			break;
		}
		entry = entry->next;
	}

	unlock_list(list);
}

int
cfulist_unshift(cfulist_t *list, void *data) {
	return cfulist_unshift_data(list, data, 0);
}

void *
cfulist_shift(cfulist_t *list) {
	void *data = NULL;
	if (cfulist_shift_data(list, &data, NULL)) {
		return data;
	}
	return NULL;
}

int
cfulist_enqueue(cfulist_t *list, void *data) {
	return cfulist_push(list, data);
}

void *
cfulist_dequeue(cfulist_t *list) {
	return cfulist_shift(list);
}

/* Dealing with strings */

int
cfulist_push_string(cfulist_t *list, char *data) {
	return cfulist_push_data(list, (void *)data, -1);
}

char *
cfulist_pop_string(cfulist_t *list) {
	void *data = NULL;
	if (cfulist_pop_data(list, &data, NULL)) {
		return (char *)data;
	}
	return NULL;
}

int
cfulist_unshift_string(cfulist_t *list, char *data) {
	return cfulist_unshift_data(list, (void *)data, -1);
}

char *
cfulist_shift_string(cfulist_t *list) {
	void *data = NULL;
	if (cfulist_shift_data(list, &data, NULL)) {
		return (char *)data;
	}
	return NULL;
}

int
cfulist_enqueue_string(cfulist_t *list, char *data) {
	return cfulist_push_string(list, data);
}

char *
cfulist_dequeue_string(cfulist_t *list) {
	return cfulist_shift_string(list);
}

typedef struct _join_foreach_struct {
	size_t count;
	const char *delimiter;
	cfustring_t *string;
} _join_foreach_struct;

static int
_join_foreach_fn(void *data, size_t data_size, void *arg) {
	_join_foreach_struct *stuff = (_join_foreach_struct *)arg;

	data_size = data_size;
	if (stuff->count) cfustring_append(stuff->string, stuff->delimiter);
	stuff->count++;
	cfustring_append(stuff->string, (char *)data);
	return 0;
}

char *
cfulist_join(cfulist_t *list, const char *delimiter) {
	_join_foreach_struct *arg = calloc(1, sizeof(_join_foreach_struct));
	char *str = NULL;

	arg->delimiter = delimiter;
	arg->string = cfustring_new();
	cfulist_foreach(list, _join_foreach_fn, (void *)arg);

	str = cfustring_get_buffer_copy(arg->string);
	cfustring_destroy(arg->string);
	free(arg);

	return str;
}

void
cfulist_destroy(cfulist_t *list) {
	if (!list) return;

	if (list->free_fn) {
		cfulist_destroy_with_free_fn(list, list->free_fn);
		return;
	}

	lock_list(list);
	if (list->entries) {
		cfulist_entry *entry = NULL;
		cfulist_entry *tmp = NULL;
		entry = list->entries;
		while (entry) {
			tmp = entry->next;
			free(entry);
			entry = tmp;
		}
	}
	unlock_list(list);
#ifdef HAVE_PTHREAD_H
	pthread_mutex_destroy(&list->mutex);
#endif
	free(list);
}

void
cfulist_destroy_with_free_fn(cfulist_t *list, cfulist_free_fn_t free_fn) {
	if (!list) return;

	lock_list(list);
	if (list->entries) {
		cfulist_entry *entry = NULL;
		cfulist_entry *tmp = NULL;
		entry = list->entries;
		while (entry) {
			tmp = entry->next;
			if (free_fn) free_fn(entry->data);
			free(entry);
			entry = tmp;
		}
	}
	unlock_list(list);
#ifdef HAVE_PTHREAD_H
	pthread_mutex_destroy(&list->mutex);
#endif
	free(list);
}
