#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([test-package], [0.0.1], [bird@dotty.su])
#AC_CONFIG_SRCDIR([main.cc])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_MKDIR_P
LT_INIT

# Checks for libraries.
# CServer
PKG_CHECK_MODULES([CServer], [CServer], [], [AC_MSG_ERROR(
             [Please install lib CServer])])
# libauth
PKG_CHECK_MODULES([libauth], [libauth], [], [AC_MSG_ERROR(
             [Please install libauth])])

AC_CHECK_LIB([crt1], [main])
# pthread
AC_CHECK_LIB([pthread], [main])
# boost libs
AC_CHECK_LIB([boost_filesystem], [main])
AC_CHECK_LIB([boost_system], [main])
AC_CHECK_LIB([boost_locale], [main])


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# AUTOMAKE
AC_CONFIG_FILES(
      [
        Makefile
        proxy/Makefile
      ])
AM_INIT_AUTOMAKE

AC_OUTPUT
