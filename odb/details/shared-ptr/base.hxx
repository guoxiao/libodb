// file      : odb/details/shared-ptr/base.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_SHARED_PTR_BASE_HXX
#define ODB_DETAILS_SHARED_PTR_BASE_HXX

#include <new>
#include <cstddef>   // std::size_t

#include <odb/exception.hxx>

namespace odb
{
  namespace details
  {
    struct share
    {
      explicit
      share (char id);

      bool
      operator== (share) const;

    private:
      char id_;
    };

    extern share shared;
    extern share exclusive;
  }
}

void*
operator new (std::size_t, odb::details::share) throw (std::bad_alloc);

void
operator delete (void*, odb::details::share) throw ();

namespace odb
{
  namespace details
  {
    struct not_shared: exception
    {
      virtual const char*
      what () const throw ();
    };

    struct shared_base
    {
      shared_base ();
      shared_base (const shared_base&);
      shared_base&
      operator= (const shared_base&);

      void
      _inc_ref ();

      bool
      _dec_ref ();

      std::size_t
      _ref_count () const;

      void*
      operator new (std::size_t, share) throw (std::bad_alloc);

      void
      operator delete (void*, share) throw ();

      void
      operator delete (void*) throw ();

      struct refcount_callback
      {
        void* arg;
        void (*zero_counter) (void*);
      };

    private:
      bool
      _dec_ref_callback ();

    protected:
      std::size_t counter_;
      refcount_callback* callback_;
    };

    template <typename X>
    inline X*
    inc_ref (X*);

    template <typename X>
    inline void
    dec_ref (X*);

    template <typename X>
    inline std::size_t
    ref_count (const X*);
  }
}

#include <odb/details/shared-ptr/base.ixx>
#include <odb/details/shared-ptr/base.txx>

#endif // ODB_DETAILS_SHARED_PTR_BASE_HXX
