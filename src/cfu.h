/*
 * cfu.h - This file is part of the libcfu library
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

#ifndef LIBCFU_H_
#define LIBCFU_H_

#ifdef __cplusplus
# define CFU_BEGIN_DECLS extern "C" {
# define CFU_END_DECLS }
# define CFU_INLINE inline
#else
# define CFU_BEGIN_DECLS
# define CFU_END_DECLS
# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  define CFU_INLINE inline
# elif defined(__GNUC__)
#  define CFU_INLINE __inline__
# else
#  define CFU_INLINE
# endif
#endif

CFU_BEGIN_DECLS

#define LIBCFU_VERSION "0.04"

typedef enum { libcfu_t_none = 0, libcfu_t_hash_table, libcfu_t_list, libcfu_t_string,
			   libcfu_t_time, libcfu_t_timer, libcfu_t_conf } libcfu_type;

typedef struct libcfu_item libcfu_item_t;

libcfu_type cfu_get_type(void *item);
int cfu_is_hash(void *item);
int cfu_is_list(void *item);
int cfu_is_string(void *item);
int cfu_is_time(void *item);
int cfu_is_timer(void *item);
int cfu_is_conf(void *item);

CFU_END_DECLS

#endif
