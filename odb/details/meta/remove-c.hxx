// file      : odb/details/meta/remove-c.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_META_REMOVE_C_HXX
#define ODB_DETAILS_META_REMOVE_C_HXX

#include <odb/pre.hxx>

namespace odb
{
  namespace details
  {
    namespace meta
    {
      template <typename X>
      struct remove_c
      {
        typedef X r;
      };

      template <typename X>
      struct remove_c<X const>
      {
        typedef X r;
      };
    }
  }
}

#include <odb/post.hxx>

#endif // ODB_DETAILS_META_REMOVE_C_HXX
