// file      : odb/traits.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_TRAITS_HXX
#define ODB_TRAITS_HXX

#include <memory>  // std::auto_ptr

#include <odb/forward.hxx>
#include <odb/shared-ptr.hxx>
#include <odb/shared-ptr-traits.hxx>

namespace odb
{
  enum id_source
  {
    ids_assigned /* Assigned by the application. */
  };

  // Specializations should defined the following members:
  //
  // id_type               - object id (primary key) type
  // id_source             - object id (primary key) source
  // id_type id (const T&) - get object id
  //
  // void insert (database&, const T&)
  // void update (database&, const T&)
  // void erase (database&, const id_type&)
  // memory_traits<T>::shared_ptr find (database&, const id_type&)
  //
  // And inherit from object_memory and object_factory.
  //
  // template <typename T>
  // class access::object_traits;

  template <typename T>
  class access::object_memory
  {
  public:
    typedef odb::shared_ptr<T> shared_ptr;
    typedef std::auto_ptr<T> unique_ptr;
  };

  template <typename T>
  class access::object_factory
  {
  public:
    static typename object_memory<T>::shared_ptr
    create ()
    {
      // By default use shared_ptr-specific construction.
      //
      return shared_factory<typename object_memory<T>::shared_ptr>::create ();
    }
  };

  template <typename P>
  class access::shared_factory
  {
  public:
    typedef typename shared_ptr_traits<P>::type object_type;

    static P
    create ()
    {
      void* v (shared_ptr_traits<P>::allocate (sizeof (object_type)));
      guard g (v);
      P p (new (v) object_type);
      g.release ();
      return p;
    }
  private:
    struct guard
    {
      guard (void* p): p_ (p) {}
      ~guard () {if (p_) shared_ptr_traits<P>::free (p_);}
      void release () {p_ = 0;}
      void* p_;
    };
  };

  template <typename T>
  struct object_traits: access::object_traits<T>
  {
    typedef
    shared_ptr_traits<typename access::object_traits<T>::shared_ptr>
    shared_ops;
  };
}

#endif // ODB_TRAITS_HXX
