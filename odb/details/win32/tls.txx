// file      : odb/details/win32/tls.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <memory> // std::auto_ptr

#include <odb/details/win32/exceptions.hxx>

using namespace std;

namespace odb
{
  namespace details
  {
    // tls<T>
    //
    template <typename T>
    once tls<T>::once_;

    template <typename T>
    size_t tls<T>::key_;

    template <typename T>
    T& tls<T>::
    get () const
    {
      once_.call (key_init);
      void* v (_get (key_));

      if (v != 0)
        return *static_cast<T*> (v);

      auto_ptr<T> p (new T);
      _set (key_, p.get ());

      T& r (*p);
      p.release ();
      return r;
    }

    template <typename T>
    void tls<T>::
    key_init ()
    {
      key_ = _allocate (destructor);
    }

    template <typename T>
    void tls<T>::
    destructor (void* v)
    {
      delete static_cast<T*> (v);
    }

    // tls<T*>
    //
    template <typename T>
    once tls<T*>::once_;

    template <typename T>
    size_t tls<T*>::key_;

    template <typename T>
    T* tls<T*>::
    get () const
    {
      once_.call (key_init);
      return static_cast<T*> (_get (key_));
    }

    template <typename T>
    void tls<T*>::
    set (T* p)
    {
      once_.call (key_init);
      _set (key_, p);
    }

    template <typename T>
    void tls<T*>::
    key_init ()
    {
      key_ = _allocate (0);
    }
  }
}
