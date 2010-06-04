// file      : odb/session.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <utility> // std::make_pair

#include <odb/session.hxx>
#include <odb/transaction.hxx>

namespace odb
{
  session::type_map::
  ~type_map ()
  {
  }

  session::object_proxy::
  ~object_proxy ()
  {
  }

  //
  // session
  //

  static session* current_session = 0;

  session::
  session ()
  {
    if (current_session != 0)
      throw already_in_session ();

    current_session = this;
  }

  bool session::
  has_current ()
  {
    return current_session != 0;
  }

  session& session::
  current ()
  {
    if (current_session == 0)
      throw not_in_session ();

    return *current_session;
  }

  void session::
  current (session& s)
  {
    current_session = &s;
  }

  void session::
  reset_current ()
  {
    current_session = 0;
  }

  void session::
  flush ()
  {
    if (!transaction::has_current ())
      throw not_in_transaction ();

    // @@ Order of insertion and deletion can be important (triggers,
    // id assignment, constraints etc).
    //

    for (object_map::iterator i (object_map_.begin ()),
           e (object_map_.end ()); i != e;)
    {
      object_proxy& pxy (*i->second);

      switch (pxy.state_)
      {
      case object_proxy::transient:
        {
          pxy.persist ();

          // If the id is auto-assigned, then we only get it now, so
          // register with the id map.
          //
          if (pxy.id_source_ != ids_assigned)
            pxy.register_id (id_map_, i);

          pxy.state_ = object_proxy::clean;
          ++i;
          break;
        }
      case object_proxy::dirty:
        {
          pxy.update ();
          pxy.state_ = object_proxy::clean;
          ++i;
          break;
        }
      case object_proxy::erased:
        {
          pxy.erase ();
          pxy.unregister_id (id_map_);
          object_map_.erase (i++);
          break;
        }
      case object_proxy::clean:
        {
          // Nothing to do for this case.
          //
          ++i;
          break;
        }
      }
    }
  }
}
