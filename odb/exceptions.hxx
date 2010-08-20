// file      : odb/exceptions.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_EXCEPTIONS_HXX
#define ODB_EXCEPTIONS_HXX

#include <odb/exception.hxx>

namespace odb
{
  struct already_in_transaction: odb::exception
  {
  };

  struct not_in_transaction: odb::exception
  {
  };

  struct transaction_already_finilized: odb::exception
  {
  };

  struct deadlock: odb::exception
  {
  };

  struct object_not_persistent: odb::exception
  {
  };

  struct object_already_persistent: odb::exception
  {
  };

  struct database_exception: odb::exception
  {
  };
}

#endif // ODB_EXCEPTIONS_HXX
