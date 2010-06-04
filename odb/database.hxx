// file      : odb/database.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DATABASE_HXX
#define ODB_DATABASE_HXX

#include <odb/traits.hxx>
#include <odb/forward.hxx>

namespace odb
{
  class transaction_impl;

  class database
  {
  public:
    virtual
    ~database ();

    template <typename T, template <typename> class P>
    typename object_traits<T>::id_type
    persist (P<T> obj);

    template <typename T>
    typename object_traits<T>::shared_ptr
    load (typename object_traits<T>::id_type const&);

    template <typename T>
    typename object_traits<T>::shared_ptr
    find (typename object_traits<T>::id_type const&);

    template <typename T, template <typename> class P>
    void
    erase (P<T> obj);

    template <typename T, template <typename> class P>
    void
    modified (P<T> obj);

    // Transaction API.
    //
  public:
    // Start a transaction. If an existing session can be obtained via
    // session::current(), the transaction is run as part of that session.
    // Otherwise a new session is created and will be automatically flushed
    // and destroyed when transaction ends.
    //
    virtual transaction_impl*
    begin_transaction () = 0;

    // Start a transaction as part of an existing session. The session
    // is not automatically flushed or destroyed when transaction ends.
    //
    virtual transaction_impl*
    begin_transaction (session&) = 0;

  protected:
    database ();

  private:
    database (const database&);
    database& operator= (const database&);
  };
}

#include <odb/database.ixx>
#include <odb/database.txx>

#endif // ODB_DATABASE_HXX
