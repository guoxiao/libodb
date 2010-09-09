// file      : odb/details/win32/once.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_WIN32_ONCE_HXX
#define ODB_DETAILS_WIN32_ONCE_HXX

#include <odb/pre.hxx>

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  undef WIN32_LEAN_AND_MEAN
#else
#  include <windows.h>
#endif

#include <odb/details/export.hxx>

namespace odb
{
  namespace details
  {
    class LIBODB_EXPORT once
    {
    public:
      once ();

      void
      call (void (*func) ());

    private:
      once (const once&);
      once& operator= (const once&);

    private:
      friend void
      once_process_start ();

      friend void
      once_process_end (bool);

    private:
      bool called_;
      static CRITICAL_SECTION cs_;
    };
  }
}

#include <odb/details/win32/once.ixx>

#include <odb/post.hxx>

#endif // ODB_DETAILS_WIN32_ONCE_HXX
