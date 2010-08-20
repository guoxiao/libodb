// file      : odb/transaction.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
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
      : finilized_ (false), impl_ (impl)
  {
    tls_set (current_transaction, this);
  }

  transaction::
  ~transaction ()
  {
    if (!finilized_)
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
    if (finilized_)
      throw transaction_already_finilized ();

    finilized_ = true;
    tls_set<transaction> (current_transaction, 0);
    impl_->commit ();
  }

  void transaction::
  rollback ()
  {
    if (finilized_)
      throw transaction_already_finilized ();

    finilized_ = true;
    tls_set<transaction> (current_transaction, 0);
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
