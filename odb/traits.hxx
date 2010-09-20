// file      : odb/traits.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_TRAITS_HXX
#define ODB_TRAITS_HXX

#include <odb/pre.hxx>

#include <odb/forward.hxx>
#include <odb/pointer-traits.hxx>

namespace odb
{
  // template <typename T>
  // class access::object_traits;
  //
  // Specializations should inherit from object_memory, object_factory
  // and define the following members:
  //
  // id_type               - object id (primary key) type
  // id_type id (const T&) - get object id
  //
  // void persist (database&, T&)
  // void update (database&, T&)
  // void erase (database&, const id_type&)
  // pointer_type find (database&, const id_type&)
  // bool find (database&, const id_type&, T&)
  //
  //

  template <typename T>
  class access::object_memory
  {
  public:
    typedef T* pointer_type;
  };

  template <typename T>
  class access::object_factory
  {
  public:
    static typename object_memory<T>::pointer_type
    create ()
    {
      // By default use pointer-specific construction.
      //
      return
        pointer_factory<typename object_memory<T>::pointer_type>::create ();
    }
  };

  template <typename P>
  class access::pointer_factory
  {
  public:
    typedef typename pointer_traits<P>::type object_type;

    static P
    create ()
    {
      void* v (pointer_traits<P>::allocate (sizeof (object_type)));
      mem_guard g (v);
      P p (new (v) object_type);
      g.release ();
      return p;
    }
  private:
    struct mem_guard
    {
      mem_guard (void* p): p_ (p) {}
      ~mem_guard () {if (p_) pointer_traits<P>::free (p_);}
      void release () {p_ = 0;}
      void* p_;
    };
  };

  template <typename T>
  struct object_traits: access::object_traits<T>
  {
    typedef
    odb::pointer_traits<typename access::object_traits<T>::pointer_type>
    pointer_traits;
  };
}

#include <odb/post.hxx>

#endif // ODB_TRAITS_HXX
