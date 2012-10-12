// file      : odb/prepared-query.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_PREPARED_QUERY_HXX
#define ODB_PREPARED_QUERY_HXX

#include <odb/pre.hxx>

#include <odb/forward.hxx>
#include <odb/traits.hxx>
#include <odb/result.hxx>
#include <odb/statement.hxx>

#include <odb/details/export.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  struct LIBODB_EXPORT prepared_query_impl: details::shared_base
  {
    virtual
    ~prepared_query_impl ();

    const char* name;
    details::shared_ptr<statement> stmt;
    details::shared_ptr<result_impl> (*execute) (prepared_query_impl&);
  };

  template <typename T>
  struct prepared_query
  {
    prepared_query () {}

    explicit
    prepared_query (details::shared_ptr<prepared_query_impl> impl)
        : impl_ (impl) {}

    result<T>
    execute (bool cache = true)
    {
      typedef
      typename result_base<T, class_traits<T>::kind>::result_impl_type
      derived_type;

      details::shared_ptr<result_impl> ri (impl_->execute (*impl_));
      result<T> r (
        details::shared_ptr<derived_type> (
          static_cast<derived_type*> (ri.release ())));

      if (cache)
        r.cache ();

      return r;
    }

    const char*
    name () const
    {
      return impl_->name;
    }

    typedef odb::statement statement_type;

    statement_type&
    statement () const
    {
      return *impl_->stmt;
    }

    typedef details::shared_ptr<prepared_query_impl>
    prepared_query::*unspecified_bool_type;
    operator unspecified_bool_type () const
    {
      return impl_ ? &prepared_query::impl_ : 0;
    }

  private:
    friend class connection;
    details::shared_ptr<prepared_query_impl> impl_;
  };

  namespace core
  {
    using odb::prepared_query;
  }
}

#include <odb/post.hxx>

#endif // ODB_PREPARED_QUERY_HXX
