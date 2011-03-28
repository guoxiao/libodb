// file      : odb/forward.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_FORWARD_HXX
#define ODB_FORWARD_HXX

#include <odb/pre.hxx>

#include <odb/details/shared-ptr-fwd.hxx>

namespace odb
{
  class database;
  class transaction;
  class session;

  template <typename T>
  class result;

  namespace core
  {
    using odb::database;
    using odb::transaction;
    using odb::session;
    using odb::result;
  }

  // Implementation details.
  //

  class access
  {
  public:
    template <typename T>
    class object_traits;

    template <typename T, typename P>
    class object_factory;

    template <typename T, typename P>
    class pointer_factory;

    template <typename T>
    class composite_value_traits;

    template <typename C>
    class container_traits;
  };

  template <typename T>
  struct object_traits;
}

#include <odb/post.hxx>

#endif // ODB_FORWARD_HXX
