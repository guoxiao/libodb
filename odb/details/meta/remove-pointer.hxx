// file      : odb/details/meta/remove-pointer.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_META_REMOVE_POINTER_HXX
#define ODB_DETAILS_META_REMOVE_POINTER_HXX

#include <odb/pre.hxx>

namespace odb
{
  namespace details
  {
    namespace meta
    {
      template <typename X>
      struct remove_pointer
      {
        typedef X result;
      };

      template <typename X>
      struct remove_pointer<X*>
      {
        typedef X result;
      };
    }
  }
}

#include <odb/post.hxx>

#endif // ODB_DETAILS_META_REMOVE_POINTER_HXX
