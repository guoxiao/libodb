// file      : odb/details/posix/mutex.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_POSIX_MUTEX_HXX
#define ODB_DETAILS_POSIX_MUTEX_HXX

#include <pthread.h>

namespace odb
{
  namespace details
  {
    class mutex
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
      pthread_mutex_t mutex_;
    };
  }
}

#include <odb/details/posix/mutex.ixx>

#endif // ODB_DETAILS_POSIX_MUTEX_HXX
