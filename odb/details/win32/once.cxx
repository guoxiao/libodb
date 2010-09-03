// file      : odb/details/win32/once.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <windows.h>

#include <odb/details/win32/once.hxx>
#include <odb/details/win32/once-init.hxx>

namespace odb
{
  namespace details
  {
    CRITICAL_SECTION once::cs_;

    void
    once_process_start ()
    {
      InitializeCriticalSection (&once::cs_);
    }

    void
    once_process_end (bool)
    {
      DeleteCriticalSection (&once::cs_);
    }
  }
}
