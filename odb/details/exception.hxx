// file      : odb/details/exception.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_EXCEPTION_HXX
#define ODB_DETAILS_EXCEPTION_HXX

#include <odb/exception.hxx>

namespace odb
{
  namespace details
  {
    struct exception: odb::exception
    {
    };
  }
}

#endif // ODB_DETAILS_EXCEPTION_HXX