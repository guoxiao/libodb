// file      : odb/transaction.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_TRANSACTION_HXX
#define ODB_TRANSACTION_HXX

#include <odb/pre.hxx>

#include <odb/forward.hxx>

#include <odb/details/export.hxx>
#include <odb/details/unique-ptr.hxx>

namespace odb
{
  class transaction_impl;

  class LIBODB_EXPORT transaction
  {
  public:
    typedef odb::database database_type;
    typedef odb::connection connection_type;

    // If the second argument is false, then this transaction is not made
    // the current transaction of the thread.
    //
    explicit
    transaction (transaction_impl*, bool make_current = true);

    // Unless the transaction has already been finalized (explicitly
    // committed or rolled back), the destructor will roll it back.
    //
    ~transaction ();

    // Unless the current transaction has already been finalized (explicitly
    // committed or rolled back), reset will roll it back. If the second
    // argument is false, then this transaction is not made the current
    // transaction of the thread.
    //
    void
    reset (transaction_impl*, bool make_current = true);

    void
    commit ();

    void
    rollback ();

    // Return the database this transaction is on.
    //
    database_type&
    database ();

    // Return the connection this transaction is on.
    //
    connection_type&
    connection ();

  public:
    // Return true if there is a transaction in effect.
    //
    static bool
    has_current ();

    // Return current transaction or throw if there is no transaction
    // in effect.
    //
    static transaction&
    current ();

    // Set the current thread's transaction.
    //
    static void
    current (transaction&);

    // Revert to the no transaction in effect state for the current thread.
    //
    static void
    reset_current ();

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
    transaction_impl&
    implementation ();

    // Copying or assignment of transactions is not supported.
    //
  private:
    transaction (const transaction&);
    transaction& operator= (const transaction&);

  protected:
    bool finalized_;
    details::unique_ptr<transaction_impl> impl_;
  };

  class LIBODB_EXPORT transaction_impl
  {
  public:
    typedef odb::database database_type;
    typedef odb::connection connection_type;

    virtual
    ~transaction_impl ();

    virtual void
    start () = 0;

    virtual void
    commit () = 0;

    virtual void
    rollback () = 0;

    database_type&
    database ()
    {
      return database_;
    }

    connection_type&
    connection ()
    {
      return *connection_;
    }

  protected:
    transaction_impl (database_type& db)
        : database_ (db), connection_ (0)
    {
    }

    transaction_impl (database_type& db, connection_type& c)
        : database_ (db), connection_ (&c)
    {
    }

  protected:
    database_type& database_;
    connection_type* connection_;
  };
}

#include <odb/transaction.ixx>

#include <odb/post.hxx>

#endif // ODB_TRANSACTION_HXX
