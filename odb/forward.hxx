// file      : odb/forward.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_FORWARD_HXX
#define ODB_FORWARD_HXX

#include <odb/pre.hxx>

#include <odb/details/export.hxx>
#include <odb/details/shared-ptr-fwd.hxx>

namespace odb
{
  class database;
  class connection;
  typedef details::shared_ptr<connection> connection_ptr;
  class transaction;
  class statement;
  class session;

  namespace core
  {
    using odb::database;
    using odb::connection;
    using odb::connection_ptr;
    using odb::transaction;
    using odb::statement;
    using odb::session;
  }

  // Tracing.
  //
  class tracer; // Not in core.
  extern LIBODB_EXPORT tracer& stderr_tracer;

  namespace core
  {
    using odb::stderr_tracer;
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

  // Cache traits.
  //
  template <typename T> struct no_id_pointer_cache_traits;
  template <typename T> struct no_op_pointer_cache_traits;
  template <typename T> struct pointer_cache_traits;
  template <typename T> struct no_id_reference_cache_traits;
  template <typename T> struct no_op_reference_cache_traits;
  template <typename T> struct reference_cache_traits;

  // Polymorphism support.
  //
  template <typename R>
  struct polymorphic_map;

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
