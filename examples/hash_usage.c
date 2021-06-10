/* Creation date: 2005-06-25 10:33:53
 * Authors: Don
 * Change log:
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "cfuhash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(HAVE_STRNCASECMP) && defined(HAVE_STRINGS_H)
# include <strings.h>
#else /* If strncasecmp() isn't available use this one */
static inline int
strncasecmp(const char *s1, const char *s2, size_t n)
{
	assert(s1 && s2);
	if (n != 0) {
		const unsigned char *us1 = (const unsigned char *) s1,
		const unsigned char *us2 = (const unsigned char *) s2;
		do {
			if (tolower(*us1) != tolower(*us2++))
				return (tolower(*us1) - tolower(*--us2));
			if (*us1++ == '\0')
				break;
		} while (--n != 0);
	}
	return 0;
}
#endif

static int
remove_func(void *key, size_t key_size, void *data, size_t data_size, void *arg) {
	(void)data_size;
	(void)data;
	(void)arg;

	if (key_size > 7) {
		if (!strncasecmp(key, "content", 7)) {
			return 1;
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	cfuhash_table_t *hash = cfuhash_new_with_initial_size(30);
	cfuhash_table_t *hash2 = cfuhash_new_with_initial_size(4);
	char *val = NULL;
	char list[32][2][32];
	size_t i;

	(void)argc;
	(void)argv;

	cfuhash_set_flag(hash, CFUHASH_FROZEN_UNTIL_GROWS);

	for (i = 0; i < 32; i++) {
		sprintf(list[i][0], "test_var%lu", i);
		sprintf(list[i][1], "value%lu", i);
	}

	cfuhash_put(hash, "var1", "value1");
	cfuhash_put(hash, "var2", "value2");
	cfuhash_put(hash, "var3", "value3");
	cfuhash_put(hash, "var4", "value4");

	cfuhash_pretty_print(hash, stdout);

	printf("\n\n");
	val = (char *)cfuhash_delete(hash, "var3");
	printf("delete: got back '%s'\n\n", val);

	cfuhash_pretty_print(hash, stdout);

	printf("\n\n");
	val = cfuhash_get(hash, "var2");
	printf("got var2='%s'\n", val);
	printf("var4 %s\n", cfuhash_exists(hash, "var4") ? "exists" : "does NOT exist!!!");

	printf("%lu entries, %lu buckets used out of %lu\n", cfuhash_num_entries(hash), cfuhash_num_buckets_used(hash), cfuhash_num_buckets(hash));

	cfuhash_pretty_print(hash, stdout);

	cfuhash_clear(hash);

	for (i = 0; i < 32; i++) {
		size_t used = cfuhash_num_buckets_used(hash);
		size_t num_buckets = cfuhash_num_buckets(hash);
		size_t num_entries = cfuhash_num_entries(hash);
		cfuhash_put(hash, list[i][0], list[i][1]);
		printf("%lu entries, %lu buckets used out of %lu (%.2f)\n", num_entries, used, num_buckets, (float)num_entries/(float)num_buckets);

	}

	cfuhash_pretty_print(hash, stdout);

	{
		char **keys = NULL;
		size_t *key_sizes = NULL;
		size_t key_count = 0;

		keys = (char **)cfuhash_keys_data(hash, &key_count, &key_sizes, 0);

		printf("\n\nkeys (%lu):\n", key_count);
		for (i = 0; i < key_count; i++) {
			printf("\t%s\n", keys[i]);
			free(keys[i]);
		}
		free(keys);
		free(key_sizes);
	}

	cfuhash_clear(hash);
	printf("%lu entries, %lu buckets used out of %lu\n", cfuhash_num_entries(hash), cfuhash_num_buckets_used(hash), cfuhash_num_buckets(hash));

	cfuhash_destroy(hash);

	printf("\n\n====> case-insensitive hash:\n\n");
	hash = cfuhash_new_with_initial_size(30);
	cfuhash_set_flag(hash, CFUHASH_IGNORE_CASE);
	cfuhash_put(hash, "Content-Type", "value1");
	cfuhash_put(hash, "Content-Length", "value2");

	cfuhash_put(hash2, "var3", "value3");
	cfuhash_put(hash2, "var4", "value4");


	cfuhash_pretty_print(hash, stdout);
	cfuhash_copy(hash2, hash);
	cfuhash_pretty_print(hash, stdout);

	{
		char keys[4][32] = { "content-type", "content-length", "Var3", "VaR4" };
		size_t i = 0;

		for (i = 0; i < 4; i++) {
			printf("%s => %s\n", keys[i], (char *)cfuhash_get(hash, keys[i]));
		}

		cfuhash_foreach_remove(hash, remove_func, NULL, NULL);
		printf("\n\nafter removing content*:\n");
		for (i = 0; i < 4; i++) {
			printf("%s => %s\n", keys[i], (char *)cfuhash_get(hash, keys[i]));
		}

	}

	cfuhash_destroy(hash);
	cfuhash_destroy(hash2);

	return 0;
}
