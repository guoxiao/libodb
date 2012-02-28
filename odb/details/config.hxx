// file      : odb/details/config.hxx
// copyright : Copyright (c) 2005-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_CONFIG_HXX
#define ODB_DETAILS_CONFIG_HXX

// no pre

#ifdef _MSC_VER
#  include <odb/details/config-vc.h>
#elif defined(ODB_COMPILER)
#  define ODB_THREADS_NONE
#  define LIBODB_STATIC_LIB
#  if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
#    define ODB_CXX11
#  endif
#else
#  include <odb/details/config.h>
#endif

// no post

#endif // ODB_DETAILS_CONFIG_HXX
