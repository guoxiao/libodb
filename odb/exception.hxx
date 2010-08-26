// file      : odb/exception.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_EXCEPTION_HXX
#define ODB_EXCEPTION_HXX

#include <odb/pre.hxx>

#include <exception>

#include <odb/details/export.hxx>

namespace odb
{
  struct LIBODB_EXPORT exception: std::exception
  {
    // By default return the exception type name ( typeid (*this).name () ).
    //
    virtual const char*
    what () const throw ();
  };
}

#include <odb/post.hxx>

#endif // ODB_EXCEPTION_HXX
