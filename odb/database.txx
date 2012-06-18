// file      : odb/database.txx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>
#include <odb/transaction.hxx>
#include <odb/no-op-cache-traits.hxx>
#include <odb/pointer-traits.hxx>

namespace odb
{
  template <typename T>
  typename object_traits<T>::id_type database::
  persist (T& obj)
  {
    // T can be const T while object_type will always be T.
    //
    typedef typename odb::object_traits<T>::object_type object_type;
    typedef odb::object_traits<object_type> object_traits;

    object_traits::persist (*this, obj);

    object_traits::reference_cache_traits::insert (
      *this, reference_cache_type<T>::convert (obj));

    return object_traits::id (obj);
  }

  template <typename T>
  typename object_traits<T>::id_type database::
  persist_ (const typename object_traits<T>::pointer_type& pobj)
  {
    // T can be const T while object_type will always be T.
    //
    typedef typename odb::object_traits<T>::object_type object_type;
    typedef odb::object_traits<object_type> object_traits;

    typedef typename odb::object_traits<T>::pointer_type pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    T& obj (pointer_traits::get_ref (pobj));
    object_traits::persist (*this, obj);

    // Get the canonical object pointer and insert it into object cache.
    //
    object_traits::pointer_cache_traits::insert (
      *this, pointer_cache_type<pointer_type>::convert (pobj));

    return object_traits::id (obj);
  }

  template <typename T>
  typename object_traits<T>::pointer_type database::
  load (const typename object_traits<T>::id_type& id)
  {
    // T is always object_type.
    //
    typedef typename object_traits<T>::pointer_type pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    pointer_type r (find<T> (id));

    if (pointer_traits::null_ptr (r))
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
  void database::
  reload (T& obj)
  {
    // T should be object_type (cannot be const).
    //
    typedef odb::object_traits<T> object_traits;

    // We don't need to check for transaction here;
    // object_traits::reload () does this.

    if (!object_traits::reload (*this, obj))
      throw object_not_persistent ();
  }

  template <typename T>
  struct database::query_<T, class_object>
  {
    static result<T>
    call (database& db, const odb::query<T>& q)
    {
      return object_traits<T>::query (db, q);
    }
  };

  template <typename T>
  struct database::query_<T, class_view>
  {
    static result<T>
    call (database& db, const odb::query<T>& q)
    {
      return view_traits<T>::query (db, q);
    }
  };

  template <typename T>
  result<T> database::
  query (const odb::query<T>& q, bool cache)
  {
    // T is always object_type.
    //

    // We don't need to check for transaction here;
    // object_traits::query () does this.

    result<T> r (query_<T, class_traits<T>::kind>::call (*this, q));

    if (cache)
      r.cache ();

    return r;
  }
}
