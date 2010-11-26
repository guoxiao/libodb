// file      : odb/tr1-pointer-traits.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_TR1_POINTER_TRAITS_HXX
#define ODB_TR1_POINTER_TRAITS_HXX

#include <odb/pre.hxx>

// This header assumes that the necessary TR1 header has already
// been included.
//
#include <odb/pointer-traits.hxx>

namespace odb
{
  // Specialization for std::tr1::shared_ptr.
  //
  template <typename T>
  class pointer_traits< std::tr1::shared_ptr<T> >
  {
  public:
    static pointer_kind const kind = pk_shared;

    typedef T element_type;
    typedef std::tr1::shared_ptr<element_type> pointer_type;
    typedef std::tr1::shared_ptr<const element_type> const_pointer_type;
    typedef smart_ptr_guard<pointer_type> guard;

    static element_type*
    get_ptr (const pointer_type& p)
    {
      return p.get ();
    }

    static element_type&
    get_ref (const pointer_type& p)
    {
      return *p;
    }

    static bool
    null_ptr (const pointer_type& p)
    {
      return p.get () == 0;
    }

  public:
    static void*
    allocate (std::size_t n)
    {
      return operator new (n);
    }

    static void
    free (void* p)
    {
      operator delete (p);
    }
  };

  // Specialization for std::tr1::weak_ptr.
  //
  template <typename T>
  class pointer_traits< std::tr1::weak_ptr<T> >
  {
  public:
    static pointer_kind const kind = pk_weak;

    typedef T element_type;
    typedef std::tr1::weak_ptr<element_type> pointer_type;
    typedef std::tr1::weak_ptr<const element_type> const_pointer_type;
    typedef std::tr1::shared_ptr<element_type> strong_pointer_type;
    typedef std::tr1::shared_ptr<const element_type> strong_const_pointer_type;
  };
}

#include <odb/post.hxx>

#endif // ODB_TR1_POINTER_TRAITS_HXX
