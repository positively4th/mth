#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_ENABLE_SHARED
AC_ENABLE_STATIC
AC_INIT(mth, 0.1, klas.sanden@yahoo.se)
AM_INIT_AUTOMAKE(mth, 0.1)
AC_CONFIG_SRCDIR([src/misc/linkedlist.h])
AC_CONFIG_HEADERS([config.h])

LT_INIT([shared])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MKTIME
AC_CHECK_FUNCS([pow sqrt strerror])
AC_CHECK_LIB([f2c])
AC_OUTPUT( \
	   Makefile \
	   src/Makefile  test/Makefile \
	   src/misc/Makefile test/misc/Makefile \
	   src/core/Makefile test/core/Makefile \
	   src/solvers/Makefile test/solvers/Makefile
	   src/fit/Makefile test/fit/Makefile \
	   src/plot/Makefile test/plot/Makefile
	   )

