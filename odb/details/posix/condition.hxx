// file      : odb/details/posix/condition.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_POSIX_CONDITION_HXX
#define ODB_DETAILS_POSIX_CONDITION_HXX

#include <pthread.h>

#include <odb/details/posix/mutex.hxx>

namespace odb
{
  namespace details
  {
    class condition
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
      pthread_cond_t cond_;
    };
  }
}

#include <odb/details/posix/condition.ixx>

#endif // ODB_DETAILS_POSIX_CONDITION_HXX
