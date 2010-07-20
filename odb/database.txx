// file      : odb/database.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>
#include <odb/transaction.hxx>

namespace odb
{
  // @@ Should I make these inline?
  //

  template <typename T>
  typename object_traits<T>::id_type database::
  persist (T& obj)
  {
    typedef object_traits<T> traits;

    if (!transaction::has_current ())
      throw not_in_transaction ();

    traits::insert (*this, obj);
    return traits::id (obj);
  }

  template <typename T>
  typename object_traits<T>::pointer_type database::
  load (typename object_traits<T>::id_type const& id)
  {
    typedef object_traits<T> traits;

    typename traits::pointer_type r (find<T> (id));

    if (traits::pointer_ops::null_ptr (r))
      throw object_not_persistent ();

    return r;
  }

  template <typename T>
  void database::
  load (typename object_traits<T>::id_type const& id, T& obj)
  {
    if (!find<T> (id, obj))
      throw object_not_persistent ();
  }

  template <typename T>
  typename object_traits<T>::pointer_type database::
  find (typename object_traits<T>::id_type const& id)
  {
    if (!transaction::has_current ())
      throw not_in_transaction ();

    return object_traits<T>::find (*this, id);
  }

  template <typename T>
  bool database::
  find (typename object_traits<T>::id_type const& id, T& obj)
  {
    if (!transaction::has_current ())
      throw not_in_transaction ();

    return object_traits<T>::find (*this, id, obj);
  }

  template <typename T>
  void database::
  store (T& obj)
  {
    if (!transaction::has_current ())
      throw not_in_transaction ();

    object_traits<T>::update (*this, obj);
  }

  template <typename T>
  void database::
  erase (const T& obj)
  {
    erase<T> (object_traits<T>::id (obj));
  }

  template <typename T>
  void database::
  erase (typename object_traits<T>::id_type const& id)
  {
    if (!transaction::has_current ())
      throw not_in_transaction ();

    object_traits<T>::erase (*this, id);
  }
}
