#sbin_PROGRAMS = server
#server_SOURCES = proxy.cc main.cc getMessage.cc setMessageAll.cc xhr.cc
#server_CPPFLAGS = $(CServer_CFLAGS)
#server_LDADD = $(CServer_LIBS)

noinst_LTLIBRARIES = libproxy.la
libproxy_la_SOURCES = proxy.cc getMessage.cc setMessageAll.cc xhr.cc
libproxy_la_CPPFLAGS = $(CServer_CFLAGS) $(libauth_CFLAGS)
#libproxy_la_LDADD = $(CServer_LIBS)
