// file      : odb/database.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>
#include <odb/session.hxx>
#include <odb/transaction.hxx>

namespace odb
{
  // @@ Should I make these inline?
  //

  template <typename T, template <typename> class P>
  typename object_traits<T>::id_type database::
  persist (P<T> p)
  {
    // P<T> should be the same or convertible to
    // object_traits<T>::shared_ptr.
    //
    const typename object_traits<T>::shared_ptr& obj (p);

    session& s (transaction::current ().session ());
    return s.persist<T> (*this, obj);
  }

  template <typename T>
  typename object_traits<T>::shared_ptr database::
  load (typename object_traits<T>::id_type const& id)
  {
    typename object_traits<T>::shared_ptr r (find<T> (id));

    if (object_traits<T>::shared_ops::null_ptr (r))
      throw object_not_persistent ();

    return r;
  }

  template <typename T>
  typename object_traits<T>::shared_ptr database::
  find (typename object_traits<T>::id_type const& id)
  {
    session& s (transaction::current ().session ());
    return s.find<T> (*this, id);
  }

  template <typename T, template <typename> class P>
  void database::
  erase (P<T> p)
  {
    // P<T> should be the same or convertible to
    // object_traits<T>::shared_ptr.
    //
    const typename object_traits<T>::shared_ptr& obj (p);

    session& s (transaction::current ().session ());
    return s.erase (*this, obj);
  }

  template <typename T, template <typename> class P>
  void database::
  modified (P<T> p)
  {
    // P<T> should be the same or convertible to
    // object_traits<T>::shared_ptr.
    //
    const typename object_traits<T>::shared_ptr& obj (p);

    session& s (transaction::current ().session ());
    return s.modified (obj);
  }
}
