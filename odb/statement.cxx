// file      : odb/statement.cxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cassert>

#include <odb/statement.hxx>

namespace odb
{
  statement::
  ~statement ()
  {
  }

  void statement::
  cached (bool cached)
  {
    assert (cached);
    cached_ = true;
  }
}
