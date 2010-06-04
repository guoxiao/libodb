// file      : odb/exception.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <typeinfo>

#include <odb/exception.hxx>

namespace odb
{
  char const* exception::
  what () const throw ()
  {
    return typeid (*this).name ();
  }
}
