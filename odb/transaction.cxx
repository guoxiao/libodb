// file      : odb/transaction.cxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/transaction.hxx>
#include <odb/exceptions.hxx>

#include <odb/details/tls.hxx>

namespace odb
{
  using namespace details;

  //
  // transaction
  //

  static ODB_TLS_POINTER (transaction) current_transaction;

  transaction::
  ~transaction ()
  {
    if (!finalized_)
      try {rollback ();} catch (...) {}
  }

  void transaction::
  reset (transaction_impl* impl, bool make_current)
  {
    details::unique_ptr<transaction_impl> i (impl);

    if (!finalized_)
      rollback ();

    impl_.reset (i.release ());

    if (make_current && tls_get (current_transaction) != 0)
      throw already_in_transaction ();

    impl_->start ();
    finalized_ = false;

    if (make_current)
      tls_set (current_transaction, this);
  }

  bool transaction::
  has_current ()
  {
    return tls_get (current_transaction) != 0;
  }

  transaction& transaction::
  current ()
  {
    transaction* cur (tls_get (current_transaction));

    if (cur == 0)
      throw not_in_transaction ();

    return *cur;
  }

  void transaction::
  current (transaction& t)
  {
    tls_set (current_transaction, &t);
  }

  void transaction::
  reset_current ()
  {
    transaction* t (0);
    tls_set (current_transaction, t);
  }

  void transaction::
  commit ()
  {
    if (finalized_)
      throw transaction_already_finalized ();

    finalized_ = true;
    impl_->connection ().transaction_tracer_ = 0;

    if (tls_get (current_transaction) == this)
    {
      transaction* t (0);
      tls_set (current_transaction, t);
    }

    impl_->commit ();
  }

  void transaction::
  rollback ()
  {
    if (finalized_)
      throw transaction_already_finalized ();

    finalized_ = true;
    impl_->connection ().transaction_tracer_ = 0;

    if (tls_get (current_transaction) == this)
    {
      transaction* t (0);
      tls_set (current_transaction, t);
    }

    impl_->rollback ();
  }

  //
  // transaction_impl
  //

  transaction_impl::
  ~transaction_impl ()
  {
  }
}
