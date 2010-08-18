// file      : odb/details/posix/tls.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_POSIX_TLS_HXX
#define ODB_DETAILS_POSIX_TLS_HXX

#include <pthread.h>

namespace odb
{
  namespace details
  {
    template <typename T>
    class tls;

    template <typename T>
    class tls
    {
    public:
      tls ();

      T&
      get () const;

    private:
      tls (const tls&);
      tls& operator= (const tls&);

    private:
      static void
      key_init ();

      static void
      destructor (void*);

    private:
      static int error_;
      static pthread_once_t once_;
      static pthread_key_t key_;
    };

    template <typename T>
    class tls<T*>
    {
    public:
      tls ();

      T*
      get () const;

      void
      set (T* p);

    private:
      tls (const tls&);
      tls& operator= (const tls&);

    private:
      static void
      key_init ();

    private:
      static int error_;
      static pthread_once_t once_;
      static pthread_key_t key_;
    };

    template <typename T>
    inline T&
    tls_get (const tls<T>& s)
    {
      return s.get ();
    }

    template <typename T>
    inline T*
    tls_get (const tls<T*>& s)
    {
      return s.get ();
    }

    template <typename T>
    inline void
    tls_set (tls<T*>& s, T* p)
    {
      return s.set (p);
    }
  }
}

#include <odb/details/posix/tls.ixx>
#include <odb/details/posix/tls.txx>

#endif // ODB_DETAILS_POSIX_TLS_HXX
