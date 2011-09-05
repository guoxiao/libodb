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
  class connection;
  typedef details::shared_ptr<connection> connection_ptr;
  class transaction;
  class session;

  namespace core
  {
    using odb::database;
    using odb::connection;
    using odb::connection_ptr;
    using odb::transaction;
    using odb::session;
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

    template <typename T>
    class view_traits;

    template <typename T, typename P>
    class view_factory;

    template <typename T, typename P>
    class pointer_factory;

    template <typename T>
    class composite_value_traits;

    template <typename C>
    class container_traits;
  };

  template <typename T>
  struct object_traits;

  template <typename T>
  struct view_traits;

  namespace details
  {
    template <>
    struct counter_type<connection>
    {
      typedef shared_base counter;
    };
  }
}

#include <odb/post.hxx>

#endif // ODB_FORWARD_HXX
