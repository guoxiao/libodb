// file      : odb/pointer-traits.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_POINTER_TRAITS_HXX
#define ODB_POINTER_TRAITS_HXX

#include <odb/pre.hxx>

#include <new>     // operators new/delete
#include <cstddef> // std::size_t

namespace odb
{
  template <typename P>
  class pointer_traits;

  // No-op pointer guard for smart pointers.
  //
  template <typename P>
  class nop_guard
  {
  public:
    nop_guard () {}

    explicit
    nop_guard (const P&) {}

    void
    release () {}

    void
    reset (const P&) {}
  };

  // Default implementation that should work for any sensible smart
  // pointer with one template argument (object type). The only
  // assumptions that we make are the availability of operator-> and
  // operator*, and that the former does not throw if the pointer is
  // NULL.
  //
  template <typename T, template <typename> class P>
  class pointer_traits< P<T> >
  {
  public:
    typedef T type;
    typedef P<T> pointer;
    typedef nop_guard<pointer> guard;

    // Return underlying pointer, including NULL.
    //
    static type*
    get_ptr (const pointer& p)
    {
      return p.operator-> ();
    }

    // Return reference to the pointed-to object.
    //
    static type&
    get_ref (const pointer& p)
    {
      return *p;
    }

    // Return true if the pointer is NULL.
    //
    static bool
    null_ptr (const pointer& p)
    {
      return get_ptr (p) == 0;
    }

  public:
    // Allocate memory for a shared object.
    //
    static void*
    allocate (std::size_t n)
    {
      return operator new (n);
    }

    // Free memory allocated for a shared object. This functions is
    // only called if the constructor of the object being created
    // fails. Otherwise, pointer is used to delete the object
    // and free the memory. This behavior is identical to the one
    // used by operator delete overloading.
    //
    static void
    free (void* p)
    {
      operator delete (p);
    }
  };

  // Specialization for naked pointer.
  //
  template <typename P>
  class nptr_guard
  {
  public:
    ~nptr_guard () {delete p_;}
    nptr_guard (): p_ (0) {}

    explicit
    nptr_guard (P p): p_ (p) {}

    void
    release () {p_ = 0;}

    void
    reset (P p) {delete p_; p_ = p;}

  private:
    P p_;
  };

  template <typename T>
  class pointer_traits<T*>
  {
  public:
    typedef T type;
    typedef T* pointer;
    typedef nptr_guard<pointer> guard;

    static type*
    get_ptr (pointer p)
    {
      return p;
    }

    static type&
    get_ref (pointer p)
    {
      return *p;
    }

    // Return true if the pointer is NULL.
    //
    static bool
    null_ptr (pointer p)
    {
      return p == 0;
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
}

#include <odb/post.hxx>

#endif // ODB_POINTER_TRAITS_HXX
