/*
 * cfustring.h - This file is part of the libcfu library
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

#ifndef CFU_STRING_H_
#define CFU_STRING_H_

#include <stdarg.h>
#include <stddef.h>
#include <cfu/cfu.h>

CFU_BEGIN_DECLS

typedef struct cfustring cfustring_t;

/* Returns a new String. */
cfustring_t * cfustring_new(void);

/* Returns a new String, but preallocates a buffer of the given size. */
cfustring_t * cfustring_new_with_initial_size(size_t initial_size);

/* Returns a new String initalized with the given string. */
cfustring_t * cfustring_new_from_string(const char *string);

/* Overwrite anything currently in cfu_str with string. */
int cfustring_dup(cfustring_t *cfu_str, const char *string);

/* Truncate the string. */
int cfustring_clear(cfustring_t *cfu_str);

/* Append str to the end of the buffer in cfu_str. */
int cfustring_append(cfustring_t *cfu_str, const char *str);

/* Get the buffer used to hold the string.  Do not free() it, as it is
 * used directly by cfustring and will be destroyed when
 * cfustring_destroy() is called.
 */
char * cfustring_get_buffer(cfustring_t *cfu_str);

/* Same as cfustring_get_buffer(), except return a copy of the string.
 * Caller is responsible for deallocating the buffer with free().
 */
char * cfustring_get_buffer_copy(cfustring_t *cfu_str);

/* Split cfu_str on one or more delimiting strings, e.g.,
 * cfustring_split(cfu_str, 2, 0, "\r\n", "\n").  Use a limit > 0 if
 * you want to only get back a certain number of strings and ignore
 * any extra delimiters.
 */
cfustring_t ** cfustring_split(cfustring_t *cfu_str, size_t *num_strings,
	size_t limit, ...);

/* Same as cfustring_split(), except return an array of C-strings.
 * Caller is responsible for deallocating the buffers.
 */
char ** cfustring_split_to_c_str(cfustring_t *cfu_str, size_t *num_strings,
	size_t limit, ...);

/* Free all resources allocated by cfu_str. */
int cfustring_destroy(cfustring_t *cfu_str);

/* Duplicate the C string str.  Caller must free with free(). */
char * cfustring_dup_c_str(const char *str);

/* Same as cfustring_dup_c_str(), but only copy at most n chars */
char * cfustring_dup_c_str_n(const char *str, size_t n);

/* Like sprintf(), but writes to a self-extending string. */
size_t cfustring_sprintf(cfustring_t *cfu_str, const char *fmt, ...);

/* Like vsprintf(), but writes to a self-extending string. */
size_t cfustring_vsprintf(cfustring_t *cfu_str, const char *fmt, va_list ap);

/* Similar to sprintf(), but allocates a C string of the
 * appropriate size for you and returns it.
 */
char * cfustring_sprintf_c_str(const char *fmt, ...);

/* Like cfustring_split_to_c_str(), but split a char * instead of a cfustring_t *. */
char ** cfustring_c_str_split(const char *c_str, size_t *num_strings, size_t limit, ...);

CFU_END_DECLS

#endif
