// file      : odb/connection.txx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

namespace odb
{
  template <typename T>
  prepared_query<T> connection::
  prepare_query (const char* n, const query<T>& q)
  {
    //@@ Views. Inline?
    //
    return prepared_query<T> (object_traits<T>::prepare_query (*this, n, q));
  }
}
