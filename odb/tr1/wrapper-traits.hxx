// file      : odb/tr1/wrapper-traits.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_TR1_WRAPPER_TRAITS_HXX
#define ODB_TR1_WRAPPER_TRAITS_HXX

#include <odb/pre.hxx>

//
// This header assumes that the necessary TR1 header has already
// been included.
//

#include <odb/wrapper-traits.hxx>

namespace odb
{
  // Specialization for std::tr1::shared_ptr.
  //
  template <typename T>
  class wrapper_traits< std::tr1::shared_ptr<T> >
  {
  public:
    typedef T wrapped_type;
    typedef std::tr1::shared_ptr<T> wrapper_type;

    static const bool null_handler = true;
    static const bool null_default = false;

    static bool
    get_null (const wrapper_type& p)
    {
      return !p;
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
      if (!p)
        p.reset (new wrapped_type);

      return *p;
    }
  };
}

#include <odb/post.hxx>

#endif // ODB_TR1_WRAPPER_TRAITS_HXX
