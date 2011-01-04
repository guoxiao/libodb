// file      : odb/exceptions.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>

namespace odb
{
  const char* null_pointer::
  what () const throw ()
  {
    return "NULL pointer";
  }

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

  const char* transaction_already_finalized::
  what () const throw ()
  {
    return "transaction already committed or rolled back";
  }

  const char* already_in_session::
  what () const throw ()
  {
    return "session already in effect in this thread";
  }

  const char* not_in_session::
  what () const throw ()
  {
    return "session not in effect in this thread";
  }

  const char* const_object::
  what () const throw ()
  {
    return "object cached in session is const";
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

  const char* result_not_cached::
  what () const throw ()
  {
    return "query result in not cached";
  }

  const char* object_already_persistent::
  what () const throw ()
  {
    return "object already persistent";
  }
}
