# file      : odb/Makefile.am
# copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

lib_LTLIBRARIES = libodb.la
libodb_la_SOURCES = __path__(sources)

odbincludedir = $(includedir)/odb
nobase_odbinclude_HEADERS = __path__(headers)
nobase_nodist_odbinclude_HEADERS = details/config.h

EXTRA_DIST = __file__(extra_dist)

# At the moment, DllMain() is only necessary if we are using Win32
# threads. So keep win32_dll_sources in ODB_THREADS_WIN32.
#
if ODB_WIN32
nobase_odbinclude_HEADERS += __path__(win32_headers)
endif

if ODB_THREADS_POSIX
libodb_la_SOURCES += __path__(posix_threads_sources)
nobase_odbinclude_HEADERS += __path__(posix_threads_headers)
endif

if ODB_THREADS_WIN32
libodb_la_SOURCES += __path__(win32_threads_sources) __path__(win32_dll_sources)
nobase_odbinclude_HEADERS += __path__(win32_threads_headers)
endif

AM_CPPFLAGS= -I'$(top_builddir)' -I'$(top_srcdir)' -DLIBODB_DYNAMIC_LIB
AM_LDFLAGS = -release __value__(interface_version) -no-undefined
