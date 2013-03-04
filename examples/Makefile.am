noinst_PROGRAMS = hash_usage large_hash strings conf_example conf_example2 opt_example
LDADD = -L$(top_srcdir)/src -lcfu

if DEBUG
AM_CFLAGS = -I$(top_srcdir)/src -DCFU_DEBUG=1
else
AM_CFLAGS = -I$(top_srcdir)/src
endif

AM_LDFLAGS =  -Xlinker -rpath -Xlinker $(top_src_dir)/src

