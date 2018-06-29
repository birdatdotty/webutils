sbin_PROGRAMS = server
server_SOURCES = main.cc
server_CPPFLAGS = $(CServer_CFLAGS)
server_LDADD = $(CServer_LIBS) $(libauth_LIBS) proxy/libproxy.la

SUBDIRS = proxy
