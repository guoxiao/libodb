// file      : odb/cache-traits.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_CACHE_TRAITS_HXX
#define ODB_CACHE_TRAITS_HXX

#include <odb/pre.hxx>

#include <odb/traits.hxx>
#include <odb/forward.hxx>
#include <odb/session.hxx>
#include <odb/pointer-traits.hxx>

namespace odb
{
  // Caching traits for objects passed by pointer.
  //
  template <typename P, typename ID, pointer_kind kind>
  struct pointer_cache_traits_impl;

  template <typename P>
  struct pointer_cache_traits: pointer_cache_traits_impl<
    P,
    typename object_traits<typename pointer_traits<P>::element_type>::id_type,
    pointer_traits<P>::kind>
  {
  };

  template <typename P, typename ID, pointer_kind kind>
  struct pointer_cache_traits_impl
  {
    typedef P pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;
    typedef typename pointer_traits::element_type element_type;

    // element_type can be const while object_type is always non-const.
    //
    typedef typename object_traits<element_type>::object_type object_type;
    typedef typename object_traits<element_type>::id_type id_type;
    typedef session::object_position<object_type> position_type;

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

    // We need the insert() overload with explicit id to handle self-
    // references. In such cases the object is not yet loaded and the
    // id member does not contain the correct id.
    //
    // Qualify the database type to resolve a phony ambiguity in VC 10.
    //
    static position_type
    insert (odb::database& db, const id_type& id, const pointer_type& p)
    {
      if (session::has_current ())
      {
        // Cast away constness if any.
        //
        return session::current ().insert<object_type> (
          db, id, pointer_traits::cast (p));
      }
      else
        return position_type ();
    }

    static position_type
    insert (odb::database& db, const pointer_type& p)
    {
      const id_type& id (
        object_traits<object_type>::id (
          pointer_traits::get_ref (p)));

      return insert (db, id, p);
    }

    static pointer_type
    find (odb::database& db, const id_type& id)
    {
      if (session::has_current ())
        return session::current ().find<object_type> (db, id);
      else
        return pointer_type ();
    }

    static void
    erase (odb::database& db, const id_type& id)
    {
      if (session::has_current ())
        session::current ().erase<object_type> (db, id);
    }

    static void
    erase (const position_type& p)
    {
      if (p.map_ != 0)
        session::current ().erase<object_type> (p);
    }
  };

  template <typename P, pointer_kind kind>
  struct pointer_cache_traits_impl<P, void, kind>
  {
    typedef P pointer_type;
    struct position_type {};

    static position_type
    insert (odb::database&, const pointer_type&)
    {
      return position_type ();
    }
  };

  // Unique pointers don't work with the object cache.
  //
  template <typename P, typename ID>
  struct pointer_cache_traits_impl<P, ID, pk_unique>
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
    insert (odb::database&, const id_type&, const pointer_type&)
    {
      return position_type ();
    }

    static position_type
    insert (odb::database&, const pointer_type&)
    {
      return position_type ();
    }

    static pointer_type
    find (odb::database&, const id_type&) { return pointer_type (); }

    static void
    erase (odb::database&, const id_type&) {}

    static void
    erase (const position_type&) {}
  };

  template <typename P>
  struct pointer_cache_traits_impl<P, void, pk_unique>
  {
    typedef P pointer_type;
    struct position_type {};

    static position_type
    insert (odb::database&, const pointer_type&)
    {
      return position_type ();
    }
  };

  // Caching traits for objects passed by reference. Only if the object
  // pointer kind is raw do we add the object to the session.
  //
  template <typename T, typename ID, pointer_kind kind>
  struct reference_cache_traits_impl;

  template <typename T>
  struct reference_cache_traits: reference_cache_traits_impl<
    T,
    typename object_traits<T>::id_type,
    pointer_traits<typename object_traits<T>::pointer_type>::kind>
  {
  };

  template <typename T, typename ID, pointer_kind kind>
  struct reference_cache_traits_impl
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
    insert (odb::database&, const id_type&, element_type&)
    {
      return position_type ();
    }

    static position_type
    insert (odb::database&, element_type&)
    {
      return position_type ();
    }
  };

  template <typename T, pointer_kind kind>
  struct reference_cache_traits_impl<T, void, kind>
  {
    typedef T element_type;
    struct position_type {};

    static position_type
    insert (odb::database&, element_type&)
    {
      return position_type ();
    }
  };

  template <typename T, typename ID>
  struct reference_cache_traits_impl<T, ID, pk_raw>
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
    insert (odb::database& db, const id_type& id, element_type& obj)
    {
      pointer_type p (&obj);
      return pointer_cache_traits<pointer_type>::insert (db, id, p);
    }

    static position_type
    insert (odb::database& db, element_type& obj)
    {
      pointer_type p (&obj);
      return pointer_cache_traits<pointer_type>::insert (db, p);
    }
  };

  template <typename T>
  struct reference_cache_traits_impl<T, void, pk_raw>
  {
    typedef T element_type;
    struct position_type {};

    static position_type
    insert (odb::database&, element_type&)
    {
      return position_type ();
    }
  };
}

#include <odb/post.hxx>

#endif // ODB_CACHE_TRAITS_HXX
