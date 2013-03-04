lib_LIBRARIES = libcfu.a

libcfu_a_SOURCES = cfuhash.c cfutime.c cfustring.c cfulist.c cfuconf.c \
                   cfu.c cfuopt.c


include_HEADERS = cfu.h cfuhash.h cfutime.h cfustring.h cfulist.h \
                  cfuconf.h cfuopt.h

if USE_PTHREADS
libcfu_a_SOURCES += cfuthread_queue.c
include_HEADERS += cfuthread_queue.h
endif

bin_PROGRAMS = libcfu-config

# TODO: provide a pkg-config .pc file instead
libcfu_config_SOURCES = libcfu-config.c
libcfu_config_CFLAGS = -DCFU_LIBDIR="\"@libdir@\"" \
                       -DCFU_LIBS="\"@LIBS@\"" \
                       -DCFU_INCLUDEDIR="\"@includedir@\"" \
                       -DCFU_PREFIX="\"@prefix@\"" \
                       -DCFU_EXEC_PREFIX="\"@exec_prefix@\""
libcfu_config_LDADD = libcfu.a
