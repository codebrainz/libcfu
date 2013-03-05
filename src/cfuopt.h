/*
 * cfuopt.h - This file is part of the libcfu library
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

#include <cfu.h>

#ifndef CFU_OPT_H_
#define CFU_OPT_H_

CFU_BEGIN_DECLS

typedef struct cfuopt_struct cfuopt_t;

/* Returns a new options context */
cfuopt_t *cfuopt_new(void);

/* Adds to the list of known options. */
void cfuopt_add_entry(cfuopt_t *context, const char *opt_str, void *arg_data,
	const char *description, const char *arg_description);

/* Parses the command line and modifies argc and argv to account for
 * left over arguments.
 */
void cfuopt_parse(cfuopt_t *context, int *argc, char ***argv, char **error);

/* Returns a help string built from the entries added with
 * cfuopt_add_entry().
 */
char * cfuopt_get_help_str(cfuopt_t *context);

/* Frees up resources used by the option parser. */
void cfuopt_destroy(cfuopt_t *context);

CFU_END_DECLS

#endif
