// file      : odb/forward.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_FORWARD_HXX
#define ODB_FORWARD_HXX

#include <odb/pre.hxx>

namespace odb
{
  class database;
  class transaction;

  class access
  {
  public:
    template <typename T>
    class object_traits;

    template <typename T>
    class object_memory;

    template <typename T>
    class object_factory;

    template <typename P>
    class pointer_factory;

    template <typename T>
    class composite_value_traits;
  };
}

#include <odb/post.hxx>

#endif // ODB_FORWARD_HXX
