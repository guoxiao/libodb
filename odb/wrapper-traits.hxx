// file      : odb/wrapper-traits.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_WRAPPER_TRAITS_HXX
#define ODB_WRAPPER_TRAITS_HXX

#include <odb/pre.hxx>

#include <memory> // std::auto_ptr

#include <odb/nullable.hxx>

namespace odb
{
  template <typename T>
  class wrapper_traits;

  // Sample specialization for raw pointers. It is not enabled by default
  // since it makes many assumptions that may not always hold true (such
  // as that instances are allocated with new and freed with delete).
  // This makes it too dangerous to be enable unconditionally. If you
  // need this functionality, you can copy the below code into your
  // application. Also consider changing it to only specialize for
  // specific types instead of for any pointer (it will almost always
  // do the wrong thing for char*).
  //
#if 0
  template <typename T>
  class wrapper_traits<T*>
  {
  public:
    typedef T wrapped_type;
    typedef T* wrapper_type;

    static const bool null_handler = true;
    static const bool null_default = false;

    static bool
    get_null (const wrapper_type& p)
    {
      return p == 0;
    }

    static void
    set_null (wrapper_type& p)
    {
      delete p;
      p = 0;
    }

    static const type&
    get_ref (const wrapper_type& p)
    {
      return *p;
    }

    static type&
    set_ref (wrapper_type& p)
    {
      if (p == 0)
        p = new type;

      return *p;
    }
  };
#endif

  // Specialization for std::auto_ptr.
  //
  template <typename T>
  class wrapper_traits< std::auto_ptr<T> >
  {
  public:
    typedef T wrapped_type;
    typedef std::auto_ptr<T> wrapper_type;

    static const bool null_handler = true;
    static const bool null_default = false;

    static bool
    get_null (const wrapper_type& p)
    {
      return p.get () == 0;
    }

    static void
    set_null (wrapper_type& p)
    {
      p.reset ();
    }

    static const wrapped_type&
    get_ref (const wrapper_type& p)
    {
      return *p;
    }

    static wrapped_type&
    set_ref (wrapper_type& p)
    {
      if (p.get () == 0)
        p.reset (new wrapped_type);

      return *p;
    }
  };

  // Specialization for odb::nullable.
  //
  template <typename T>
  class wrapper_traits< nullable<T> >
  {
  public:
    typedef T wrapped_type;
    typedef nullable<T> wrapper_type;

    static const bool null_handler = true;
    static const bool null_default = true;

    static bool
    get_null (const wrapper_type& n)
    {
      return n.null ();
    }

    static void
    set_null (wrapper_type& n)
    {
      n.reset ();
    }

    static const wrapped_type&
    get_ref (const wrapper_type& n)
    {
      return *n;
    }

    static wrapped_type&
    set_ref (wrapper_type& n)
    {
      if (n.null ())
        n = T ();

      return *n;
    }
  };
}

#include <odb/post.hxx>

#endif // ODB_WRAPPER_TRAITS_HXX
