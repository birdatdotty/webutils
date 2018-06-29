lib_LTLIBRARIES = libCServer.la
libCServer_la_SOURCES = CServer.cc
libCServer_la_LDFLAGS = -version-info 3:12:1

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ../pkgconfig/CServer.pc

otherincludedir = $(includedir)/$(PACKAGE)
otherinclude_HEADERS = CServer.h Callback.h account.h reducer.h
