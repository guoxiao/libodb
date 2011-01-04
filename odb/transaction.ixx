// file      : odb/transaction.ixx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

namespace odb
{
  inline transaction::database_type& transaction::
  database ()
  {
    return impl_->database ();
  }

  inline transaction_impl& transaction::
  implementation ()
  {
    return *impl_;
  }
}
