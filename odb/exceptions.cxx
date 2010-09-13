// file      : odb/exceptions.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>

namespace odb
{
  const char* already_in_transaction::
  what () const throw ()
  {
    return "transaction already in progress in this thread";
  }

  const char* not_in_transaction::
  what () const throw ()
  {
    return "operation can only be performed in transaction";
  }

  const char* transaction_already_finilized::
  what () const throw ()
  {
    return "transaction already committed or rolled back";
  }

  const char* deadlock::
  what () const throw ()
  {
    return "transaction aborted due to deadlock";
  }

  const char* object_not_persistent::
  what () const throw ()
  {
    return "object not persistent";
  }

  const char* object_already_persistent::
  what () const throw ()
  {
    return "object already persistent";
  }
}
