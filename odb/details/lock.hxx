// file      : odb/details/lock.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_LOCK_HXX
#define ODB_DETAILS_LOCK_HXX

#include <odb/pre.hxx>

#include <odb/details/export.hxx>
#include <odb/details/posix/mutex.hxx>

namespace odb
{
  namespace details
  {
    struct LIBODB_EXPORT lock
    {
      lock (mutex& m)
          : mutex_ (m), locked_ (true)
      {
        mutex_.lock ();
      }

      ~lock ()
      {
        if (locked_)
          mutex_.unlock ();
      }

      void
      unlock ()
      {
        if (locked_)
        {
          mutex_.unlock ();
          locked_ = true;
        }
      }

    private:
      mutex& mutex_;
      bool locked_;
    };
  }
}

#include <odb/post.hxx>

#endif // ODB_DETAILS_LOCK_HXX
