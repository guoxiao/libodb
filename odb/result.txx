// file      : odb/result.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

namespace odb
{
  template <typename T>
  result_impl<T>::
  ~result_impl ()
  {
  }

  template <typename T>
  typename result_impl<T>::pointer_type& result_impl<T>::
  current ()
  {
    if (pointer_traits::null_ptr (current_) && !end_)
    {
      current (traits::create ());
      current (pointer_traits::get_ref (current_));
    }

    return current_;
  }
}
