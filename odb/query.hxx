// file      : odb/query.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_QUERY_HXX
#define ODB_QUERY_HXX

#include <string>

#include <odb/forward.hxx>
#include <odb/traits.hxx>

namespace odb
{
  template <typename T>
  class query: public object_traits<T>::query_type
  {
  public:
    typedef typename object_traits<T>::query_type base_type;

    query ()
    {
    }

    query (const std::string& q)
        : base_type (q)
    {
    }

    query (const base_type& q)
        : base_type (q)
    {
    }
  };
}

#endif // ODB_QUERY_HXX
