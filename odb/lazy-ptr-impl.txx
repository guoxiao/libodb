// file      : odb/lazy-ptr-impl.txx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/database.hxx>

namespace odb
{
  //
  // lazy_ptr_base
  //

  template <typename T>
  void lazy_ptr_base::
  free (void* p)
  {
    delete static_cast<T*> (p);
  }

  template <typename T>
  void* lazy_ptr_base::
  copy (const void* p)
  {
    return new T (*static_cast<const T*> (p));
  }

  //
  // lazy_ptr_impl
  //

  template <typename T>
  template <typename O>
  inline typename object_traits<O>::pointer_type lazy_ptr_impl<T>::
  load (bool reset)
  {
    typedef typename object_traits<T>::id_type id_type;
    typedef typename object_traits<T>::pointer_type pointer_type;

    const id_type& id (*static_cast<const id_type*> (id_));
    pointer_type p (db_->load<T> (id));

    if (reset)
      reset_id ();

    // If you get a compile error pointing here, then you most likely
    // used a wrong type as a template argument in the load() call.
    //
    return p;
  }
}
