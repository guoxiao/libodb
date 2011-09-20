// file      : odb/query.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_QUERY_HXX
#define ODB_QUERY_HXX

#include <odb/pre.hxx>

#include <odb/traits.hxx>

namespace odb
{
  //
  //
  template <typename T>
  struct query_columns_base;

  template <typename T, const char* table>
  struct query_columns;

  template <typename T, const char* table>
  struct pointer_query_columns;

  // Object pointer syntax wrapper.
  //
  template <typename T>
  struct query_pointer
  {
    query_pointer ()
    {
      // For some reason GCC needs this dummy c-tor if we make a static
      // data member of this type const.
    }

    T*
    operator-> () const
    {
      return 0; // All members in T are static.
    }
  };

  //
  //
  template <typename T, class_kind kind = class_traits<T>::kind>
  struct query_selector;

  template <typename T>
  struct query_selector<T, class_object>
  {
    typedef typename object_traits<T>::query_base_type base_type;
    typedef typename object_traits<T>::query_type type;
  };

  template <typename T>
  struct query_selector<T, class_view>
  {
    typedef typename view_traits<T>::query_base_type base_type;
    typedef typename view_traits<T>::query_type type;
  };

  template <typename T, typename Q = typename query_selector<T>::base_type>
  class query;

  namespace core
  {
    using odb::query;
  }
}

#include <odb/post.hxx>

#endif // ODB_QUERY_HXX
