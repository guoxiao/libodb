// file      : odb/database.ixx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

namespace odb
{
  inline database::
  database ()
  {
  }

  template <typename T>
  inline void database::
  erase (const T& obj)
  {
    erase<T> (object_traits<T>::id (obj));
  }

  template <typename T>
  inline result<T> database::
  query (bool cache)
  {
    return query (odb::query<T> (), cache);
  }

  template <typename T>
  inline result<T> database::
  query (const char* q, bool cache)
  {
    return query (odb::query<T> (q), cache);
  }

  template <typename T>
  inline result<T> database::
  query (const std::string& q, bool cache)
  {
    return query (odb::query<T> (q), cache);
  }
}
