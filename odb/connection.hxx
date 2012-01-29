// file      : odb/connection.hxx
// copyright : Copyright (c) 2005-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_CONNECTION_HXX
#define ODB_CONNECTION_HXX

#include <odb/pre.hxx>

#include <string>
#include <cstddef> // std::size_t

#include <odb/forward.hxx>

#include <odb/details/export.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  class transaction_impl;

  class connection;
  typedef details::shared_ptr<connection> connection_ptr;

  class LIBODB_EXPORT connection: public details::shared_base
  {
  public:
    typedef odb::database database_type;

    database_type&
    database ();

    // Transactions.
    //
  public:
    virtual transaction_impl*
    begin () = 0;

    // Native database statement execution. Note that unlike the
    // versions in the database class, these can be executed
    // without a transaction.
    //
  public:
    unsigned long long
    execute (const char* statement);

    unsigned long long
    execute (const std::string& statement);

    virtual unsigned long long
    execute (const char* statement, std::size_t length) = 0;

    // SQL statement tracing.
    //
  public:
    typedef odb::tracer tracer_type;

    void
    tracer (tracer_type&);

    void
    tracer (tracer_type*);

    tracer_type*
    tracer () const;

   public:
    // Store the transaction-spacific tracer in the connection. If we
    // were to store it in the transaction, then in order to check if
    // it was set, we would need to get the transaction instance using
    // the current() API. But that requires a TLS lookup, which can be
    // slow.
    //
    tracer_type*
    transaction_tracer () const;

  public:
    virtual
    ~connection ();

  protected:
    connection (database_type&);

  private:
    connection (const connection&);
    connection& operator= (const connection&);

  protected:
    database_type& database_;
    tracer_type* tracer_;

  protected:
    friend class transaction;
    tracer_type* transaction_tracer_;
  };
}

#include <odb/connection.ixx>

#include <odb/post.hxx>

#endif // ODB_MYSQL_CONNECTION_HXX
