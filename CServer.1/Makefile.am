pkglib_LTLIBRARIES = libCServer.la
libCServer_la_SOURCES = CServer.cc
libCServer_la_LDFLAGS = -version-info 3:12:1

otherincludedir = $(includedir)/$(PACKAGE)
otherinclude_HEADERS = Callback.h Reducer.h CServer.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ../pkgconfig/CServer.pc
