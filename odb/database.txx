// file      : odb/database.txx
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>
#include <odb/transaction.hxx>
#include <odb/session.hxx>
#include <odb/callback.hxx>
#include <odb/cache-traits.hxx>
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

    if (!transaction::has_current ())
      throw not_in_transaction ();

    object_traits::callback (*this, obj, callback_event::pre_persist);
    object_traits::persist (*this, obj);
    object_traits::callback (*this, obj, callback_event::post_persist);

    reference_cache_traits<T>::insert (*this, obj);

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

    if (!transaction::has_current ())
      throw not_in_transaction ();

    T& obj (pointer_traits::get_ref (pobj));

    object_traits::callback (*this, obj, callback_event::pre_persist);
    object_traits::persist (*this, obj);
    object_traits::callback (*this, obj, callback_event::post_persist);

    pointer_cache_traits<pointer_type>::insert (*this, pobj);

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

    if (!object_traits::reload (*this, obj))
      throw object_not_persistent ();
  }

  template <typename T>
  typename object_traits<T>::pointer_type database::
  find (const typename object_traits<T>::id_type& id)
  {
    // T is always object_type.
    //
    typedef odb::object_traits<T> object_traits;
    typedef typename object_traits::pointer_type pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    // First check the session.
    //
    {
      pointer_type p (
        pointer_cache_traits<pointer_type>::find (*this, id));

      if (!pointer_traits::null_ptr (p))
        return p;
    }

    if (!transaction::has_current ())
      throw not_in_transaction ();

    // Compiler error pointing here? Perhaps the object doesn't have the
    // default constructor?
    //
    return pointer_type (object_traits::find (*this, id));
  }

  template <typename T>
  bool database::
  find (const typename object_traits<T>::id_type& id, T& obj)
  {
    // T is always object_type.
    //
    if (!transaction::has_current ())
      throw not_in_transaction ();

    return object_traits<T>::find (*this, id, obj);
  }

  template <typename T>
  void database::
  update (T& obj)
  {
    // T can be const T while object_type will always be T.
    //
    typedef typename odb::object_traits<T>::object_type object_type;
    typedef odb::object_traits<object_type> object_traits;

    if (!transaction::has_current ())
      throw not_in_transaction ();

    object_traits::callback (*this, obj,callback_event::pre_update);

    // Compiler error pointing here? Perhaps the object is readonly or
    // doesn't have an object id? Such objects cannot be updated.
    //
    object_traits::update (*this, obj);

    object_traits::callback (*this, obj, callback_event::post_update);
  }

  template <typename T>
  void database::
  update_ (const typename object_traits<T>::pointer_type& pobj)
  {
    // T can be const T while object_type will always be T.
    //
    typedef typename odb::object_traits<T>::object_type object_type;
    typedef odb::object_traits<object_type> object_traits;

    typedef typename odb::object_traits<T>::pointer_type pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    if (!transaction::has_current ())
      throw not_in_transaction ();

    T& obj (pointer_traits::get_ref (pobj));

    object_traits::callback (*this, obj, callback_event::pre_update);

    // Compiler error pointing here? Perhaps the object is readonly or
    // doesn't have an object id? Such objects cannot be updated.
    //
    object_traits::update (*this, obj);

    object_traits::callback (*this, obj, callback_event::post_update);
  }

  template <typename T>
  void database::
  erase (const typename object_traits<T>::id_type& id)
  {
    // T is always object_type.
    //
    typedef odb::object_traits<T> object_traits;
    typedef typename object_traits::pointer_type pointer_type;

    if (!transaction::has_current ())
      throw not_in_transaction ();

    object_traits::erase (*this, id);
    pointer_cache_traits<pointer_type>::erase (*this, id);
  }

  template <typename T>
  void database::
  erase (T& obj)
  {
    // T can be const T while object_type will always be T.
    //
    typedef typename odb::object_traits<T>::object_type object_type;
    typedef odb::object_traits<object_type> object_traits;
    typedef typename object_traits::pointer_type pointer_type;

    if (!transaction::has_current ())
      throw not_in_transaction ();

    typename object_traits::id_type id (object_traits::id (obj));

    object_traits::callback (*this, obj, callback_event::pre_erase);
    object_traits::erase (*this, obj);
    pointer_cache_traits<pointer_type>::erase (*this, id);
    object_traits::callback (*this, obj, callback_event::post_erase);
  }

  template <typename T>
  unsigned long long database::
  erase_query (const odb::query<T>& q)
  {
    // T is always object_type.
    //
    if (!transaction::has_current ())
      throw not_in_transaction ();

    return object_traits<T>::erase_query (*this, q);
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
    if (!transaction::has_current ())
      throw not_in_transaction ();

    result<T> r (query_<T, class_traits<T>::kind>::call (*this, q));

    if (cache)
      r.cache ();

    return r;
  }
}
