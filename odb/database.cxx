// file      : odb/database.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/database.hxx>

namespace odb
{
  database::
  ~database ()
  {
  }

  unsigned long long database::
  execute (const char* st, std::size_t n)
  {
    connection_type& c (transaction::current ().connection ());
    return c.execute (st, n);
  }
}
