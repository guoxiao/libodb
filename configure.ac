# file      : configure.ac
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

AC_PREREQ(2.60)
AC_INIT([libodb], [__value__(version)], [odb-users@codesynthesis.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([odb/version.hxx])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects dist-bzip2 dist-zip tar-ustar])

LT_INIT([win32-dll])

AC_CANONICAL_HOST

# Check for C++ compiler and use it to compile the tests.
#
AC_PROG_CXX
AC_LANG(C++)

# Check for threads.
#
AC_ARG_ENABLE(
  [threads],
  AS_HELP_STRING([--disable-threads], [disable threads (enabled by default)]),
  [AS_IF([test x"$enableval" = xno], [threads=none], [threads=check])],
  [threads=check])

# If thread support is not disabled by the user, figure out what we
# can use.
#
AS_IF(
  [test x$threads = xcheck],
  [
   case $host_os in
     windows* | mingw*)
       AC_DEFINE([ODB_THREADS_WIN32], [1], [Have Win32 threads.])
       case $host_os in
         mingw*)
	   CXXFLAGS="$CXXFLAGS -mthreads"
	   ;;
       esac
       threads=win32
       ;;
     *)
       ACX_PTHREAD
       AS_IF(
         [test x$acx_pthread_ok = xyes],
         [
          threads=posix
          LIBS="$LIBS $PTHREAD_LIBS"
          CXXFLAGS="$CXXFLAGS $PTHREAD_CXXFLAGS"
          AC_DEFINE([ODB_THREADS_POSIX], [1], [Have POSIX threads.])
         ])
       ;;
   esac
  ],
  [AC_DEFINE([ODB_THREADS_NONE], [1], [Have no threads.])])

AM_CONDITIONAL([ODB_THREADS_NONE], [test x"$threads" = xnone])
AM_CONDITIONAL([ODB_THREADS_WIN32], [test x"$threads" = xwin32])
AM_CONDITIONAL([ODB_THREADS_POSIX], [test x"$threads" = xposix])

# Check if we should disable rpath.
#
AC_MSG_CHECKING([whether to use rpath])
AC_ARG_ENABLE(
  [rpath],
  [AC_HELP_STRING([--disable-rpath], [patch libtool to not use rpath])],
  [libtool_rpath="$enable_rpath"],
  [libtool_rpath="yes"])
AC_MSG_RESULT($libtool_rpath)

# Allow the user to specify the pkgconfig directory.
#
AC_ARG_WITH(
  [pkgconfigdir],
  [AC_HELP_STRING([--with-pkgconfigdir=DIR],[location of pkgconfig dir (default is libdir/pkgconfig)])],
  [pkgconfigdir=${withval}],
  [pkgconfigdir='${libdir}/pkgconfig'])
AC_SUBST([pkgconfigdir])

# Required by subdir-objects.
#
AM_PROG_CC_C_O

# Patch libtool to not use rpath if requested.
#
AC_CONFIG_COMMANDS(
  [libtool-rpath-patch],
  [if test "$libtool_use_rpath" = "no"; then
     sed < libtool > libtool-2 's/^hardcode_libdir_flag_spec.*$'/'hardcode_libdir_flag_spec=" -D__LIBTOOL_NO_RPATH__ "/'
     mv libtool-2 libtool
     chmod 755 libtool
  fi],
  [libtool_use_rpath=$libtool_rpath])

# Output.
#
AC_CONFIG_HEADERS([config.h odb/details/config.h])
AC_CONFIG_FILES([
  __path__(config_files)
])
AC_OUTPUT

AS_IF(
  [test x$threads = xcheck],
  [
    AC_MSG_NOTICE
    AC_MSG_NOTICE([warning: thread support not available, building single-threaded])
  ])
