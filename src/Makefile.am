lib_LIBRARIES = libcfu.a

libcfu_a_SOURCES = cfuhash.c cfutime.c cfustring.c cfulist.c cfuconf.c \
                   cfuthread_queue.c cfu.c cfuopt.c

include_HEADERS = cfu.h cfuhash.h cfutime.h cfustring.h cfulist.h \
                  cfuconf.h cfuthread_queue.h cfuopt.h

bin_PROGRAMS = libcfu-config

# TODO: provide a pkg-config .pc file instead
libcfu_config_SOURCES = libcfu-config.c
libcfu_config_CFLAGS = -DCFU_LIBDIR="\"@libdir@\"" \
                       -DCFU_LIBS="\"@LIBS@\"" \
                       -DCFU_INCLUDEDIR="\"@includedir@\"" \
                       -DCFU_PREFIX="\"@prefix@\"" \
                       -DCFU_EXEC_PREFIX="\"@exec_prefix@\""
libcfu_config_LDADD = libcfu.a
