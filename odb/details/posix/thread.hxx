// file      : odb/details/posix/thread.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_POSIX_THREAD_HXX
#define ODB_DETAILS_POSIX_THREAD_HXX

#include <pthread.h>

namespace odb
{
  namespace details
  {
    class thread
    {
    public:
      ~thread ();
      thread (void* (*thread_func) (void*), void* arg = 0);

      void*
      join ();

    private:
      thread (const thread&);
      thread& operator= (const thread&);

    private:
      bool detached_;
      pthread_t id_;
    };
  }
}

#include <odb/details/posix/thread.ixx>

#endif // ODB_DETAILS_POSIX_THREAD_HXX