// file      : odb/exception.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_EXCEPTION_HXX
#define ODB_EXCEPTION_HXX

#include <exception>

namespace odb
{
  struct exception: std::exception
  {
    // By default return the exception type name ( typeid (*this).name () ).
    //
    virtual const char*
    what () const throw ();
  };
}

#endif // ODB_EXCEPTION_HXX
