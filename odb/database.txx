// file      : odb/database.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>
#include <odb/transaction.hxx>

namespace odb
{
  template <typename T>
  typename object_traits<T>::id_type database::
  persist (const T& obj)
  {
    typedef object_traits<T> traits;

    if (!transaction::has_current ())
      throw not_in_transaction ();

    traits::persist (*this, obj);
    return traits::id (obj);
  }

  template <typename T>
  typename object_traits<T>::id_type database::
  persist (T& obj)
  {
    typedef object_traits<T> traits;

    if (!transaction::has_current ())
      throw not_in_transaction ();

    traits::persist (*this, obj);
    return traits::id (obj);
  }

  template <typename T>
  typename object_traits<T>::pointer_type database::
  load (const typename object_traits<T>::id_type& id)
  {
    typedef object_traits<T> traits;
    typename traits::pointer_type r (find<T> (id));

    if (traits::pointer_traits::null_ptr (r))
      throw object_not_persistent ();

    return r;
  }

  template <typename T>
  void database::
  load (const typename object_traits<T>::id_type& id, T& obj)
  {
    if (!find<T> (id, obj))
      throw object_not_persistent ();
  }

  template <typename T>
  typename object_traits<T>::pointer_type database::
  find (const typename object_traits<T>::id_type& id)
  {
    if (!transaction::has_current ())
      throw not_in_transaction ();

    // Compiler error pointing here? Perhaps the object doesn't have the
    // default constructor?
    //
    return object_traits<T>::find (*this, id);
  }

  template <typename T>
  bool database::
  find (const typename object_traits<T>::id_type& id, T& obj)
  {
    if (!transaction::has_current ())
      throw not_in_transaction ();

    return object_traits<T>::find (*this, id, obj);
  }

  template <typename T>
  void database::
  update (const T& obj)
  {
    if (!transaction::has_current ())
      throw not_in_transaction ();

    object_traits<T>::update (*this, obj);
  }

  template <typename T>
  void database::
  erase (const typename object_traits<T>::id_type& id)
  {
    if (!transaction::has_current ())
      throw not_in_transaction ();

    object_traits<T>::erase (*this, id);
  }

  template <typename T>
  result<T> database::
  query (const odb::query<T>& q, bool cache)
  {
    if (!transaction::has_current ())
      throw not_in_transaction ();

    result<T> r (object_traits<T>::query (*this, q));

    if (cache)
      r.cache ();

    return r;
  }
}
