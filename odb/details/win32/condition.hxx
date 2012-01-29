// file      : odb/details/win32/condition.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_WIN32_CONDITION_HXX
#define ODB_DETAILS_WIN32_CONDITION_HXX

#include <odb/pre.hxx>

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  undef WIN32_LEAN_AND_MEAN
#else
#  include <windows.h>
#endif

#include <cstddef> // std::size_t

#include <odb/details/export.hxx>
#include <odb/details/win32/mutex.hxx>

namespace odb
{
  namespace details
  {
    class LIBODB_EXPORT condition
    {
    public:
      ~condition ();
      condition (mutex&);

      void
      signal ();

      void
      wait ();

    private:
      condition (const condition&);
      condition& operator= (const condition&);

    private:
      mutex& mutex_;
      HANDLE event_;

      std::size_t waiters_;
      std::size_t signals_;
    };
  }
}

#include <odb/details/win32/condition.ixx>

#include <odb/post.hxx>

#endif // ODB_DETAILS_WIN32_CONDITION_HXX
