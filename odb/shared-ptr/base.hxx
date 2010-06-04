// file      : odb/shared-ptr/base.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SHARED_PTR_BASE_HXX
#define ODB_SHARED_PTR_BASE_HXX

#include <new>
#include <cstddef>   // std::size_t

#include <odb/exception.hxx>

namespace odb
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
}

extern odb::share shared;
extern odb::share exclusive;

void*
operator new (std::size_t, odb::share) throw (std::bad_alloc);

void
operator delete (void*, odb::share) throw ();

namespace odb
{
  struct not_shared: exception
  {
    virtual char const*
    what () const throw ();
  };

  struct shared_base
  {
    shared_base ();
    shared_base (shared_base const&);
    shared_base&
    operator= (shared_base const&);

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

  protected:
    std::size_t counter_;
  };

  template <typename X>
  inline X*
  inc_ref (X*);

  template <typename X>
  inline void
  dec_ref (X*);

  template <typename X>
  inline std::size_t
  ref_count (X const*);
}

#include <odb/shared-ptr/base.ixx>
#include <odb/shared-ptr/base.txx>

#endif // ODB_SHARED_PTR_BASE_HXX
