// file      : odb/session.ixx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>

namespace odb
{
  template <typename T>
  inline void session::
  erase (const object_position<T>& p)
  {
    // @@ Empty maps are not cleaned up by this version of erase.
    //
    p.map_->erase (p.pos_);
  }

  //
  // object_pointers
  //
  template <typename T>
  inline session::object_pointers<T>::
  object_pointers () : p_ (), cp_ () {}

  template <typename T>
  inline void session::object_pointers<T>::
  set (const pointer_type& p)
  {
    p_ = p;
    cp_ = const_pointer_type ();
  }

  template <typename T>
  inline void session::object_pointers<T>::
  set (const const_pointer_type& cp)
  {
    p_ = pointer_type ();
    cp_ = cp;
  }

  template <typename T>
  inline void session::object_pointers<T>::
  get (pointer_type& p) const
  {
    if (!pointer_traits<pointer_type>::null_ptr (p_))
      p = p_;
    else if (!pointer_traits<const_pointer_type>::null_ptr (cp_))
      throw const_object ();
    else
      p = pointer_type ();
  }

  template <typename T>
  inline void session::object_pointers<T>::
  get (const_pointer_type& cp) const
  {
    if (!pointer_traits<pointer_type>::null_ptr (p_))
      cp = const_pointer_type (p_);
    else if (!pointer_traits<const_pointer_type>::null_ptr (cp_))
      cp = cp_;
    else
      cp = const_pointer_type ();
  }
}
