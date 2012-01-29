// file      : odb/details/win32/mutex.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_WIN32_MUTEX_HXX
#define ODB_DETAILS_WIN32_MUTEX_HXX

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
    class LIBODB_EXPORT mutex
    {
    public:
      ~mutex ();
      mutex ();

      void
      lock ();

      void
      unlock ();

    private:
      mutex (const mutex&);
      mutex& operator= (const mutex&);

    private:
      friend class condition;
      CRITICAL_SECTION cs_;
    };
  }
}

#include <odb/details/win32/mutex.ixx>

#include <odb/post.hxx>

#endif // ODB_DETAILS_WIN32_MUTEX_HXX
