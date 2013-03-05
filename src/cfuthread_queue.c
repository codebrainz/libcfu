/*
 * cfuthread_queue.c - This file is part of the libcfu library
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

#include "cfuthread_queue.h"
#include "cfulist.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct cfuthread_queue {
	pthread_mutex_t mutex;
	pthread_cond_t cv;
	cfulist_t *request_queue;
	cfuthread_queue_fn_t fn;
	pthread_t thread;
	cfuthread_queue_init_t init_fn;
	void *init_arg;
	cfuthread_queue_cleanup_t cleanup_fn;
	void *cleanup_arg;
};

typedef struct cfuthread_queue_entry {
	pthread_mutex_t mutex;
	pthread_cond_t cv;
	void *data_in;
	void *data_out;
} cfuthread_queue_entry;

static cfuthread_queue_entry *
_new_cfuthread_entry(void *data) {
	cfuthread_queue_entry *entry = calloc(1, sizeof(cfuthread_queue_entry));
	pthread_mutex_init(&entry->mutex, NULL);
	pthread_cond_init(&entry->cv, NULL);
	entry->data_in = data;
	return entry;
}

static void
_destroy_cfuthread_entry(cfuthread_queue_entry *entry) {
	pthread_mutex_destroy(&entry->mutex);
	pthread_cond_destroy(&entry->cv);
	free(entry);
}

static void *
_run_queue(void *arg) {
	cfuthread_queue_t *tq = (cfuthread_queue_t *)arg;
	cfuthread_queue_entry *request = NULL;

	if (tq->init_fn) {
		tq->init_fn(tq->init_arg);
	}

	pthread_cleanup_push(tq->cleanup_fn, tq->cleanup_arg);

	while (1) {
		pthread_mutex_lock(&tq->mutex);
		while (cfulist_num_entries(tq->request_queue) == 0) {
			pthread_cond_wait(&tq->cv, &tq->mutex);
		}

		request = (cfuthread_queue_entry *)cfulist_dequeue(tq->request_queue);
		pthread_mutex_unlock(&tq->mutex);
		if (!request) continue;

		pthread_mutex_lock(&request->mutex);
		request->data_out = tq->fn(request->data_in);
		pthread_cond_signal(&request->cv);
		pthread_mutex_unlock(&request->mutex);
	}
	pthread_exit((void *)0);

	pthread_cleanup_pop(0);

}

cfuthread_queue_t *
cfuthread_queue_new_with_cleanup(cfuthread_queue_fn_t fn, cfuthread_queue_init_t init_fn,
	void *init_arg, cfuthread_queue_cleanup_t cleanup_fn,
	void *cleanup_arg) {
	cfuthread_queue_t *tq = calloc(1, sizeof(cfuthread_queue_t));
	pthread_mutex_init(&tq->mutex, NULL);
	pthread_cond_init(&tq->cv, NULL);
	tq->fn = fn;
	tq->request_queue = cfulist_new();
	tq->init_fn = init_fn;
	tq->init_arg = init_arg;
	tq->cleanup_fn = cleanup_fn;
	tq->cleanup_arg = cleanup_arg;

	/* FIXME: do pthread_create() here to run a function that waits on
	   an entry to be put into the queue, then call fn().
	*/
	if ( (0 != pthread_create(&tq->thread, NULL, _run_queue, (void *)tq)) ) {
		return NULL;
	}

	return tq;
}

cfuthread_queue_t *
cfuthread_queue_new(cfuthread_queue_fn_t fn) {
	return cfuthread_queue_new_with_cleanup(fn, NULL, NULL, NULL, NULL);
}

void *
cfuthread_queue_make_request(cfuthread_queue_t * tq, void *data) {
	cfuthread_queue_entry *request = _new_cfuthread_entry(data);

	pthread_mutex_lock(&tq->mutex);
	pthread_mutex_lock(&request->mutex);
	cfulist_enqueue(tq->request_queue, (void *)request);
	pthread_cond_signal(&tq->cv);
	pthread_mutex_unlock(&tq->mutex);

	pthread_cond_wait(&request->cv, &request->mutex);
	pthread_mutex_unlock(&request->mutex);

	data = request->data_out;
	_destroy_cfuthread_entry(request);

	return data;
}

void
cfuthread_queue_destroy(cfuthread_queue_t *tq) {
	void *rv = NULL;

	pthread_cancel(tq->thread);
	pthread_join(tq->thread, &rv);
	pthread_mutex_destroy(&tq->mutex);
	pthread_cond_destroy(&tq->cv);
	cfulist_destroy(tq->request_queue);
	free(tq);
}
