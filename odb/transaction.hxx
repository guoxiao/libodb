// file      : odb/transaction.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_TRANSACTION_HXX
#define ODB_TRANSACTION_HXX

#include <odb/pre.hxx>

#include <odb/forward.hxx>

#include <odb/details/export.hxx>

namespace odb
{
  class transaction_impl;

  class LIBODB_EXPORT transaction
  {
  public:
    typedef odb::database database_type;

    explicit
    transaction (transaction_impl*);

    // Unless the transaction has already been finilized (explicitly
    // committed or rolled back), the destructor will roll it back.
    //
    ~transaction ();

    void
    commit ();

    void
    rollback ();

    // Return the database this transaction is on.
    //
    database_type&
    database ();

    // Return current transaction or throw if there is no transaction
    // in effect.
    //
    static transaction&
    current ();

    // Return true if there is a transaction in effect.
    //
    static bool
    has_current ();

  public:
    transaction_impl&
    implementation ();

    // Copying or assignment of transactions is not supported.
    //
  private:
    transaction (const transaction&);
    transaction& operator= (const transaction&);

  protected:
    bool finilized_;
    transaction_impl* impl_;
  };

  class LIBODB_EXPORT transaction_impl
  {
  protected:
    friend class transaction;

    typedef odb::database database_type;

    transaction_impl (database_type& db)
        : database_ (db)
    {
    }

    virtual
    ~transaction_impl ();

    virtual void
    commit () = 0;

    virtual void
    rollback () = 0;

    database_type&
    database ()
    {
      return database_;
    }

  protected:
    database_type& database_;
  };
}

#include <odb/transaction.ixx>

#include <odb/post.hxx>

#endif // ODB_TRANSACTION_HXX
