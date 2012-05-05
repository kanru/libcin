#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

AC_PREREQ([2.68])
AC_INIT([libcin], [0.1.0])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])
LT_INIT
AC_CONFIG_SRCDIR([src/trie.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# increment if the interface has additions, changes, removals.
LIBCIN_CURRENT=0

# increment any time the source changes; set to 0 if you increment CURRENT
LIBCIN_REVISION=0

# increment if any interfaces have been added; set to 0
# if any interfaces have been removed. removal has 
# precedence over adding, so set to 0 if both happened.
LIBCIN_AGE=0

AC_SUBST(LIBCIN_CURRENT)
AC_SUBST(LIBCIN_REVISION)
AC_SUBST(LIBCIN_AGE)


# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([strchr strdup])

AC_CONFIG_FILES([
  Makefile
])
AC_OUTPUT
