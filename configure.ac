AC_PREREQ([2.67])
AC_INIT([libcfu], [0.04], [mbrush@codebrainz.ca])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_SRCDIR([src/cfu.c])
AM_INIT_AUTOMAKE([foreign -Wall silent-rules dist-bzip2])
AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_RANLIB

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_ASSERT
AC_HEADER_TIME
AC_CHECK_HEADERS([sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([gettimeofday memset strcasecmp strncasecmp])

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

# Allow enabling debug mode using configure argument
AC_ARG_ENABLE(debug,
  [  --enable-debug    Turn on debugging],
    [case "${enableval}" in
      yes) debug=true ;;
      no)  debug=false ;;
      *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
    esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  examples/Makefile
  doc/Makefile
])

AC_OUTPUT
