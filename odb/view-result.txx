// file      : odb/view-result.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

namespace odb
{
  //
  // result_impl
  //

  template <typename T>
  result_impl<T, class_view>::
  ~result_impl ()
  {
  }

  template <typename T>
  typename result_impl<T, class_view>::pointer_type&
  result_impl<T, class_view>::
  current ()
  {
    if (pointer_traits::null_ptr (current_) && !end_)
    {
      pointer_type p (view_traits::create ());
      view_type& view (pointer_traits::get_ref (p));
      current (p);
      load (view);
    }

    return current_;
  }

  //
  // result_iterator
  //

  template <typename T>
  void result_iterator<T, class_view>::
  load (view_type& view)
  {
    if (res_->end ())
      return;

    res_->load (view);
  }
}
