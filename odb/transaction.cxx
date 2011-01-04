// file      : odb/transaction.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>
#include <odb/transaction.hxx>

#include <odb/details/tls.hxx>

namespace odb
{
  using namespace details;

  //
  // transaction
  //

  static ODB_TLS_POINTER (transaction) current_transaction;

  transaction::
  transaction (transaction_impl* impl)
      : finalized_ (false), impl_ (impl)
  {
    tls_set (current_transaction, this);
  }

  transaction::
  ~transaction ()
  {
    if (!finalized_)
    {
      try
      {
        rollback ();
      }
      catch (...)
      {
      }
    }

    delete impl_;
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
  commit ()
  {
    if (finalized_)
      throw transaction_already_finalized ();

    finalized_ = true;
    transaction* t (0);
    tls_set (current_transaction, t);
    impl_->commit ();
  }

  void transaction::
  rollback ()
  {
    if (finalized_)
      throw transaction_already_finalized ();

    finalized_ = true;
    transaction* t (0);
    tls_set (current_transaction, t);
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
