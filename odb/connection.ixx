// file      : odb/connection.ixx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cstring> // std::string

namespace odb
{
  inline connection::
  connection (database_type& database)
      : database_ (database)
  {
  }

  inline connection::database_type& connection::
  database ()
  {
    return database_;
  }

  inline unsigned long long connection::
  execute (const char* st)
  {
    return execute (st, std::strlen (st));
  }

  inline unsigned long long connection::
  execute (const std::string& st)
  {
    return execute (st.c_str (), st.size ());
  }
}
