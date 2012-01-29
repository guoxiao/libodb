// file      : odb/connection.ixx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cstring> // std::string

namespace odb
{
  inline connection::
  connection (database_type& database)
      : database_ (database), tracer_ (0), transaction_tracer_ (0)
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

  inline void connection::
  tracer (tracer_type& t)
  {
    tracer_ = &t;
  }

  inline void connection::
  tracer (tracer_type* t)
  {
    tracer_ = t;
  }

  inline connection::tracer_type* connection::
  tracer () const
  {
    return tracer_;
  }

  inline connection::tracer_type* connection::
  transaction_tracer () const
  {
    return transaction_tracer_;
  }
}
