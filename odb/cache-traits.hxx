// file      : odb/cache-traits.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_CACHE_TRAITS_HXX
#define ODB_CACHE_TRAITS_HXX

#include <odb/pre.hxx>

#include <odb/traits.hxx>
#include <odb/session.hxx>
#include <odb/pointer-traits.hxx>

namespace odb
{
  // Caching traits for objects passed by pointer.
  //
  template <typename P, pointer_kind = pointer_traits<P>::kind>
  struct pointer_cache_traits
  {
    typedef P pointer_type;
    typedef typename pointer_traits<pointer_type>::element_type element_type;
    typedef typename object_traits<element_type>::id_type id_type;
    typedef session::object_position<element_type> position_type;

    struct insert_guard
    {
      insert_guard (const position_type& pos): pos_ (pos) {}
      ~insert_guard () {erase (pos_);}

      position_type
      position () const {return pos_;}

      void
      release () {pos_.map_ = 0;}

    private:
      position_type pos_;
    };

    static position_type
    insert (database& db, const id_type& id, const pointer_type& p)
    {
      if (session::has_current ())
        return session::current ().insert<element_type> (db, id, p);
      else
        return position_type ();
    }

    static pointer_type
    find (database& db, const id_type& id)
    {
      if (session::has_current ())
        return session::current ().find<element_type> (db, id);
      else
        return pointer_type ();
    }

    static void
    erase (database& db, const id_type& id)
    {
      if (session::has_current ())
        session::current ().erase<element_type> (db, id);
    }

    static void
    erase (const position_type& p)
    {
      if (p.map_ != 0)
        session::current ().erase<element_type> (p);
    }
  };

  // Unique pointers don't work with the object cache.
  //
  template <typename P>
  struct pointer_cache_traits<P, pk_unique>
  {
    typedef P pointer_type;
    typedef typename pointer_traits<pointer_type>::element_type element_type;
    typedef typename object_traits<element_type>::id_type id_type;
    struct position_type {};

    struct insert_guard
    {
      insert_guard (const position_type&) {}

      position_type
      position () const {return position_type ();}

      void
      release () {}
    };

    static position_type
    insert (database&, const id_type&, const pointer_type&)
    {
      return position_type ();
    }

    static pointer_type
    find (database&, const id_type&) { return pointer_type (); }

    static void
    erase (database&, const id_type&) {}

    static void
    erase (const position_type&) {}
  };

  // Caching traits for objects passed by reference. Only if the object
  // pointer kind is raw do we add the object to the session.
  //
  template <typename T,
            pointer_kind =
              pointer_traits<typename object_traits<T>::pointer_type>::kind>
  struct reference_cache_traits
  {
    typedef T element_type;
    typedef typename object_traits<element_type>::pointer_type pointer_type;
    typedef typename object_traits<element_type>::id_type id_type;

    typedef
    typename pointer_cache_traits<pointer_type>::position_type
    position_type;

    struct insert_guard
    {
      insert_guard (const position_type&) {}

      position_type
      position () const {return position_type ();}

      void
      release () {}
    };

    static position_type
    insert (database&, const id_type&, element_type&)
    {
      return position_type ();
    }
  };

  template <typename T>
  struct reference_cache_traits<T, pk_raw>
  {
    typedef T element_type;
    typedef typename object_traits<element_type>::pointer_type pointer_type;
    typedef typename object_traits<element_type>::id_type id_type;

    typedef
    typename pointer_cache_traits<pointer_type>::position_type
    position_type;

    typedef
    typename pointer_cache_traits<pointer_type>::insert_guard
    insert_guard;

    static position_type
    insert (database& db, const id_type& id, element_type& obj)
    {
      pointer_type p (&obj);
      return pointer_cache_traits<pointer_type>::insert (db, id, p);
    }
  };
}

#include <odb/post.hxx>

#endif // ODB_CACHE_TRAITS_HXX
