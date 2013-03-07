AC_PREREQ([2.67])
AC_INIT([libcfu], [0.04], [mbrush@codebrainz.ca])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_SRCDIR([src/cfu.c])
AM_INIT_AUTOMAKE([foreign -Wall silent-rules dist-bzip2])
AM_SILENT_RULES([yes])
LT_INIT

AC_PROG_CC
AM_PROG_CC_C_O

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_ASSERT
AC_HEADER_TIME
AC_CHECK_HEADERS([stdarg.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([gettimeofday memset snprintf strcasecmp strncasecmp vsnprintf])

# Check for clock_gettime()
AC_CHECK_FUNCS([clock_gettime], [],
               [AC_CHECK_LIB([rt], [clock_gettime],
                             [AC_DEFINE(HAVE_CLOCK_GETTIME, 1)
                              AC_SUBST([REALTIME_LIBS], [-lrt])])])

# Check for pthread support
AC_CHECK_LIB([pthread],
             [pthread_create],
             [AC_CHECK_HEADERS([pthread.h],
                               [have_pthreads=yes],
                               [have_pthreads=no])],
             [have_pthreads=no])
if test "$have_pthreads" = "yes"
then
  AC_SUBST([PTHREAD_LIBS], [-lpthread])
fi
AM_CONDITIONAL([USE_PTHREADS], [test x$have_pthreads = xyes])

AC_CONFIG_FILES([
  libcfu.pc
  Makefile
  src/Makefile
  examples/Makefile
  doc/Makefile
])

AC_OUTPUT
