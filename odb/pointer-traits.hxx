// file      : odb/pointer-traits.hxx
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_POINTER_TRAITS_HXX
#define ODB_POINTER_TRAITS_HXX

#include <odb/pre.hxx>

#include <new>     // operators new/delete
#include <memory>  // std::auto_ptr
#include <cstddef> // std::size_t

#include <odb/details/meta/remove-const.hxx>

namespace odb
{
  enum pointer_kind
  {
    pk_raw,    // Raw pointer or equivalent (i.e., unmanaged).
    pk_unique, // Smart pointer that doesn't support sharing.
    pk_shared, // Smart pointer that supports sharing.
    pk_weak    // Weak counterpart for shared pointer.
  };

  template <typename P>
  class pointer_traits;

  //
  // Standard pointer guards.
  //

  // Raw pointer guard.
  //
  template <typename P>
  class raw_ptr_guard
  {
  public:
    ~raw_ptr_guard () {delete p_;}
    raw_ptr_guard (): p_ (0) {}

    explicit
    raw_ptr_guard (P p): p_ (p) {}

    void
    release () {p_ = 0;}

    void
    reset (P p) {delete p_; p_ = p;}

  private:
    P p_;
  };

  // No-op pointer guard for smart pointers.
  //
  template <typename P>
  class smart_ptr_guard
  {
  public:
    smart_ptr_guard () {}

    explicit
    smart_ptr_guard (const P&) {}

    void
    release () {}

    void
    reset (const P&) {}
  };

  // Specialization for raw pointers.
  //
  template <typename T>
  class pointer_traits<T*>
  {
  public:
    static const pointer_kind kind = pk_raw;
    static const bool lazy = false;

    typedef T element_type;
    typedef T* pointer_type;
    typedef const T* const_pointer_type;
    typedef typename odb::details::meta::remove_const<T>::result*
    unrestricted_pointer_type;
    typedef raw_ptr_guard<pointer_type> guard;

    // Return raw pointer to the pointed-to element, including NULL.
    //
    static element_type*
    get_ptr (pointer_type p)
    {
      return p;
    }

    // Return reference to the pointed-to element.
    //
    static element_type&
    get_ref (pointer_type p)
    {
      return *p;
    }

    // Return true if the pointer is NULL.
    //
    static bool
    null_ptr (pointer_type p)
    {
      return p == 0;
    }

    // Cast away constness.
    //
    static unrestricted_pointer_type
    cast (pointer_type p)
    {
      return const_cast<unrestricted_pointer_type> (p);
    }

  public:
    // Allocate memory for an element that will be managed by this
    // pointer.
    //
    static void*
    allocate (std::size_t n)
    {
      return operator new (n);
    }

    // Free memory allocated for an element. This functions is only
    // called if the constructor of the element being created fails.
    // Otherwise, the pointer (or guard) is used to delete the object
    // and free the memory. This behavior is identical to the one
    // used by operator delete overloading.
    //
    static void
    free (void* p)
    {
      operator delete (p);
    }
  };

  // Specialization for std::auto_ptr.
  //
  template <typename T>
  class pointer_traits< std::auto_ptr<T> >
  {
  public:
    static const pointer_kind kind = pk_unique;
    static const bool lazy = false;

    typedef T element_type;
    typedef std::auto_ptr<element_type> pointer_type;
    typedef std::auto_ptr<const element_type> const_pointer_type;
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

    // cast() is not provided since it transfers the ownership.
    //

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
