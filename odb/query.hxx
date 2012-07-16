// file      : odb/query.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_QUERY_HXX
#define ODB_QUERY_HXX

#include <odb/pre.hxx>

#include <odb/traits.hxx>

namespace odb
{
  // Table alias for type T and alias tag Tag. The dummy third template
  // argument is used to make the C++ compiler weed out duplicates.
  //
  // The alias_traits interface consists of two things: the table_name
  // static variable containing the name and, in case of a derived type
  // in a polymorphic hierarchy, the base_traits typedef. Note that the
  // same interface is exposed by object_traits, which is used when
  // we need straight tables instead of aliases.
  //
  //
  template <typename T, typename Tag, bool dummy = true>
  struct alias_traits;

  template <typename T>
  struct query_columns_base;

  template <typename T, typename Alias>
  struct query_columns;

  template <typename T, typename Alias>
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

  // VC9 cannot handle certain cases of non-type arguments with default
  // values in template functions (e.g., database::query()). As a result,
  // we have to use the impl trick below instead of simply having kind
  // as a second template argument with a default value.
  //
  template <typename T, class_kind kind>
  struct query_selector_impl;

  template <typename T>
  struct query_selector_impl<T, class_object>
  {
    typedef typename object_traits<T>::query_base_type base_type;
    typedef query_columns<T, access::object_traits<T> > columns_type;
  };

  template <typename T>
  struct query_selector_impl<T, class_view>
  {
    typedef typename view_traits<T>::query_base_type base_type;
    typedef typename view_traits<T>::query_columns columns_type;
  };

  template <typename T>
  struct query_selector: query_selector_impl<T, class_traits<T>::kind>
  {
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
