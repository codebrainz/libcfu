lib_LIBRARIES = libcfu.a

libcfu_a_SOURCES = cfuhash.c cfutime.c cfustring.c cfulist.c cfuconf.c \
                   cfu.c cfuopt.c


libcfuincdir = $(includedir)/cfu
libcfuinc_HEADERS = cfu.h cfuhash.h cfutime.h cfustring.h cfulist.h \
                    cfuconf.h cfuopt.h

if USE_PTHREADS
libcfu_a_SOURCES += cfuthread_queue.c
libcfuinc_HEADERS += cfuthread_queue.h
endif
