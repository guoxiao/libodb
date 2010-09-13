// file      : odb/exceptions.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_EXCEPTIONS_HXX
#define ODB_EXCEPTIONS_HXX

#include <odb/pre.hxx>

#include <odb/exception.hxx>

#include <odb/details/export.hxx>

namespace odb
{
  struct LIBODB_EXPORT already_in_transaction: odb::exception
  {
    virtual const char*
    what () const throw ();
  };

  struct LIBODB_EXPORT not_in_transaction: odb::exception
  {
    virtual const char*
    what () const throw ();
  };

  struct LIBODB_EXPORT transaction_already_finilized: odb::exception
  {
    virtual const char*
    what () const throw ();
  };

  struct LIBODB_EXPORT deadlock: odb::exception
  {
    virtual const char*
    what () const throw ();
  };

  struct LIBODB_EXPORT object_not_persistent: odb::exception
  {
    virtual const char*
    what () const throw ();
  };

  struct LIBODB_EXPORT object_already_persistent: odb::exception
  {
    virtual const char*
    what () const throw ();
  };

  struct LIBODB_EXPORT database_exception: odb::exception
  {
  };
}

#include <odb/post.hxx>

#endif // ODB_EXCEPTIONS_HXX
