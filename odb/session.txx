// file      : odb/session.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <utility> // std::make_pair

#include <odb/exceptions.hxx>
#include <odb/transaction.hxx>

#include <iostream> // @@ tmp

namespace odb
{
  //
  // session::object_proxy_impl
  //

  template <typename T>
  void session::object_proxy_impl<T>::
  persist ()
  {
    traits::insert (db_, ptr_ops::get_ref (obj_));
  }

  template <typename T>
  void session::object_proxy_impl<T>::
  update ()
  {
    traits::update (db_, ptr_ops::get_ref (obj_));
  }

  template <typename T>
  void session::object_proxy_impl<T>::
  erase ()
  {
    traits::erase (db_, traits::id (ptr_ops::get_ref (obj_)));
  }

  template <typename T>
  void session::object_proxy_impl<T>::
  register_id (id_map& idm, object_map::iterator i)
  {
    // For polymorphic types we should use the root of the hierarchy
    // (presumably defined in traits) as a type id so that all types
    // in this hierarchy end up in the same map. See also other places.
    //
    shared_ptr<type_map>& m (idm[&typeid (T)]);

    if (!m)
      m.reset (new (shared) type_map_impl<T>);

    type_map_impl<T>& mi (static_cast<type_map_impl<T>&> (*m));

    std::pair<typename type_map_impl<T>::iterator, bool> r (
      mi.insert (std::make_pair (traits::id (ptr_ops::get_ref (obj_)), i)));

    if (!r.second)
      throw object_already_persistent ();
  }

  template <typename T>
  void session::object_proxy_impl<T>::
  unregister_id (id_map& idm)
  {
    shared_ptr<type_map>& m (idm[&typeid (T)]);

    if (m)
    {
      type_map_impl<T>& mi (static_cast<type_map_impl<T>&> (*m));
      mi.erase (traits::id (ptr_ops::get_ref (obj_)));
    }
  }

  //
  // session
  //

  template <typename T, template <typename> class P>
  typename object_traits<T>::id_type session::
  persist (database& db, P<T> p)
  {
    typedef object_traits<T> traits;
    typedef typename traits::shared_ops ops;

    // P<T> should be the same or convertible to
    // object_traits<T>::shared_ptr.
    //
    const typename traits::shared_ptr& obj (p);

    // For polymorphic types we need to cast the pointer to the
    // root of the hierarchy.
    //
    void* ptr (ops::get_ptr (obj));
    T& ref (ops::get_ref (obj));

    // @@ What if the object that's already in the map is in the
    //    erased state?
    //
    if (object_map_.find (ptr) != object_map_.end ())
      throw object_already_persistent ();

    shared_ptr<object_proxy> pxy (new (shared) object_proxy_impl<T> (db, obj));

    // If we are in a transaction, save this object immediately. This
    // helps with two things: (1) assignment of auto-generated ids and
    // (2) detection of duplicate objects.
    //
    if (transaction::has_current ())
    {
      // @@ What if the manually-assigned id is already in use by
      //    an object waiting to be deleted?
      //
      traits::insert (db, ref);
      pxy->state_ = object_proxy::clean;
    }
    else
      pxy->state_ = object_proxy::transient;

    object_map::iterator i (
      object_map_.insert (std::make_pair (ptr, pxy)).first);

    // If this object has auto-assigned id and we haven't hit the db
    // yet, then the id is "NULL" and we cannot insert this object
    // into the id map.
    //
    if (pxy->state_ != object_proxy::transient ||
        traits::id_source == ids_assigned)
    {
      try
      {
        pxy->register_id (id_map_, i);
      }
      catch (...)
      {
        object_map_.erase (i);
        throw;
      }
    }

    return traits::id (ref);
  }

  template <typename T>
  typename object_traits<T>::shared_ptr session::
  find (database& db, typename object_traits<T>::id_type const& id)
  {
    typedef object_traits<T> traits;
    typedef typename traits::shared_ops ops;
    typedef typename traits::shared_ptr obj_ptr;

    // First see if we have this object in the maps.
    //
    shared_ptr<type_map>& m (id_map_[&typeid (T)]);

    if (m)
    {
      typedef type_map_impl<T> map_impl;
      map_impl& mi (static_cast<map_impl&> (*m));
      typename map_impl::iterator i (mi.find (id));

      if (i != mi.end ())
      {
        const object_map::iterator& j (i->second);

        if (j->second->state_ != object_proxy::erased)
        {
          object_proxy_impl<T>& pxy (
            static_cast<object_proxy_impl<T>&> (*j->second));
          return pxy.obj_;
        }
        else
          return obj_ptr ();
      }
    }

    // If we are in transaction then hit the database. Otherwise, the
    // object is not found.
    //
    if (!transaction::has_current ())
      return obj_ptr ();

    obj_ptr obj (traits::find (db, id));

    if (ops::null_ptr (obj))
      return obj;

    // Add this object to our maps.
    //
    void* ptr (ops::get_ptr (obj));
    shared_ptr<object_proxy> pxy (new (shared) object_proxy_impl<T> (db, obj));
    pxy->state_ = object_proxy::clean;

    object_map::iterator i (
      object_map_.insert (
        std::make_pair (ptr, pxy)).first);

    try
    {
      if (!m)
        m.reset (new (shared) type_map_impl<T>);

      type_map_impl<T>& mi (static_cast<type_map_impl<T>&> (*m));
      mi.insert (std::make_pair (id, i));
    }
    catch (...)
    {
      object_map_.erase (i);
      throw;
    }

    return obj;
  }

  template <typename T, template <typename> class P>
  void session::
  erase (database& db, P<T> p)
  {
    typedef object_traits<T> traits;
    typedef typename traits::shared_ops ops;

    // P<T> should be the same or convertible to
    // object_traits<T>::shared_ptr.
    //
    const typename traits::shared_ptr& obj (p);

    // For polymorphic types we need to cast the pointer to the
    // root of the hierarchy.
    //
    void* ptr (ops::get_ptr (obj));
    T& ref (ops::get_ref (obj));

    object_map::iterator i (object_map_.find (ptr));

    if (object_map_.find (ptr) == object_map_.end ())
      throw object_not_persistent ();

    object_proxy& pxy (*i->second);

    switch (pxy.state_)
    {
    case object_proxy::transient:
      {
        // If this object is still transient then all we need to do is
        // purge it from the maps.
        //

        // See if we are registered in the id map.
        //
        if (traits::id_source == ids_assigned)
          pxy.unregister_id (id_map_);

        object_map_.erase (i);
        break;
      }
    case object_proxy::clean:
    case object_proxy::dirty:
      {
        // Ideally we would need to store the object id as well as the
        // version (optimistic concurrency) since the user is free to
        // modify the object state.
        //
        pxy.state_ = object_proxy::erased;
        break;
      }
    case object_proxy::erased:
      {
        // Already erased. Throw to be consistent with the transient
        // case.
        //
        throw object_not_persistent ();
      }
    }
  }

  template <typename T, template <typename> class P>
  void session::
  modified (P<T> p)
  {
    typedef object_traits<T> traits;
    typedef typename traits::shared_ops ops;

    // P<T> should be the same or convertible to
    // object_traits<T>::shared_ptr.
    //
    const typename traits::shared_ptr& obj (p);

    // For polymorphic types we need to cast the pointer to the
    // root of the hierarchy.
    //
    void* ptr (ops::get_ptr (obj));

    object_map::iterator i (object_map_.find (ptr));

    if (object_map_.find (ptr) == object_map_.end ())
      throw object_not_persistent ();

    object_proxy& pxy (*i->second);

    switch (pxy.state_)
    {
    case object_proxy::transient:
    case object_proxy::dirty:
      {
        // Nothing to do here.
        //
        break;
      }
    case object_proxy::clean:
      {
        pxy.state_ = object_proxy::dirty;
        break;
      }
    case object_proxy::erased:
      {
        throw object_not_persistent ();
      }
    }
  }
}
