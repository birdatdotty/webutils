#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([webutils], [0.0.12], [bird@dotty.su])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
LT_INIT

# Checks for libraries.
PKG_CHECK_MODULES([JSONCPP], [jsoncpp], [],
                  [AC_MSG_ERROR([Please install jsoncpp-devel])])

PKG_CHECK_MODULES([LIBSSH], [libssh], [],
                  [AC_MSG_ERROR([Please install libssh-devel])])

AC_CHECK_LIB([LIBQXX], [libqxx])
#                  [AC_MSG_ERROR([Please install libpqxx-devel])])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.


AC_CONFIG_FILES([
Makefile
auth/Makefile
base64/Makefile
saveFile/Makefile
helperJSON/Makefile
ssh/Makefile
createHTML/Makefile
postgres/Makefile
uploadFiles/Makefile
CServer/Makefile

pkgconfig/libauth.pc
pkgconfig/libbase64.pc
pkgconfig/libsaveFile.pc
pkgconfig/libhelperJSON.pc
pkgconfig/libsshCmd.pc
pkgconfig/libCpg.pc
pkgconfig/libcreateHTML.pc
pkgconfig/uploadFiles.pc
pkgconfig/CServer.pc
])

AM_INIT_AUTOMAKE

AC_OUTPUT
