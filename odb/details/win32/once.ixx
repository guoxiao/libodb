// file      : odb/details/win32/once.ixx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/win32/lock.hxx>

namespace odb
{
  namespace details
  {
    inline once::
    once ()
        : called_ (false)
    {
    }

    inline void once::
    call (void (*func) ())
    {
      win32_lock l (cs_);

      if (!called_)
      {
        func ();
        called_ = true;
      }
    }
  }
}
