// file      : odb/meta/remove-p.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_META_REMOVE_P_HXX
#define ODB_META_REMOVE_P_HXX

namespace odb
{
  namespace meta
  {
    template <typename X>
    struct remove_p
    {
      typedef X r;
    };

    template <typename X>
    struct remove_p<X*>
    {
      typedef X r;
    };
  }
}

#endif // ODB_META_REMOVE_P_HXX
