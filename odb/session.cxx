// file      : odb/session.cxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>
#include <odb/session.hxx>

#include <odb/details/tls.hxx>

namespace odb
{
  using namespace details;

  static ODB_TLS_POINTER (session) current_session;

  session::
  session (bool make_current)
  {
    if (make_current)
    {
      if (has_current ())
        throw already_in_session ();

      current (*this);
    }
  }

  session::
  ~session ()
  {
    // If we are the current thread's session, reset it.
    //
    if (has_current () && &current () == this)
      reset_current ();
  }

  bool session::
  has_current ()
  {
    return tls_get (current_session) != 0;
  }

  session& session::
  current ()
  {
    session* cur (tls_get (current_session));

    if (cur == 0)
      throw not_in_session ();

    return *cur;
  }

  void session::
  current (session& s)
  {
    tls_set (current_session, &s);
  }

  void session::
  reset_current ()
  {
    session* s (0);
    tls_set (current_session, s);
  }

  //
  // object_map_base
  //
  session::object_map_base::
  ~object_map_base ()
  {
  }
}
