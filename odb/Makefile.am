# file      : odb/Makefile.am
# copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

lib_LTLIBRARIES = libodb.la
libodb_la_SOURCES = __path__(sources)

odbincludedir = $(includedir)/odb
nobase_odbinclude_HEADERS = __path__(headers)
nobase_nodist_odbinclude_HEADERS = details/config.h

EXTRA_DIST = __file__(extra_dist)

if ODB_THREADS_POSIX
libodb_la_SOURCES += __path__(posix_sources)
nobase_odbinclude_HEADERS += __path__(posix_headers)
endif

if ODB_THREADS_WIN32
libodb_la_SOURCES += __path__(win32_sources) __path__(win32_dll_sources)
nobase_odbinclude_HEADERS += __path__(win32_headers)
endif

AM_CPPFLAGS= -I'$(top_builddir)' -I'$(top_srcdir)' -DLIBODB_DYNAMIC_LIB
AM_LDFLAGS = -release __value__(interface_version) -no-undefined
