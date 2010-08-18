// file      : odb/details/tls.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_TLS_HXX
#define ODB_DETAILS_TLS_HXX

#include <odb/details/posix/tls.hxx>

#define ODB_TLS_POINTER(type) tls<type*>
#define ODB_TLS_OBJECT(type) tls<type>

#endif // ODB_DETAILS_TLS_HXX
