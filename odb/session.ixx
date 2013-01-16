// file      : odb/session.ixx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>

namespace odb
{
  template <typename T>
  inline void session::
  erase (const position<T>& p)
  {
    // @@ Empty maps are not cleaned up by this version of erase.
    //
    if (p.map_ != 0)
      p.map_->erase (p.pos_);
  }
}
