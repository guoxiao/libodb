// file      : odb/object-result.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/session.hxx>
#include <odb/cache-traits.hxx>

namespace odb
{
  //
  // object_result_impl
  //

  template <typename T>
  object_result_impl<T>::
  ~object_result_impl ()
  {
  }

  template <typename T>
  typename object_result_impl<T>::pointer_type&
  object_result_impl<T>::
  current ()
  {
    if (pointer_traits::null_ptr (current_) && !end_)
    {
      if (!session::has_current ())
      {
        pointer_type p (object_traits::create ());
        object_type& obj (pointer_traits::get_ref (p));
        current (p);
        load (obj);
      }
      else
      {
        // First check the session.
        //
        const id_type& id (load_id ());

        pointer_type p (
          pointer_cache_traits<pointer_type>::find (database (), id));

        if (!pointer_traits::null_ptr (p))
          current (p);
        else
        {
          pointer_type p (object_traits::create ());

          typename pointer_cache_traits<pointer_type>::insert_guard ig (
            pointer_cache_traits<pointer_type>::insert (database (), id, p));

          object_type& obj (pointer_traits::get_ref (p));
          current (p);
          load (obj);
          ig.release ();
        }
      }
    }

    return current_;
  }

  //
  // object_result_impl_no_id
  //
  template <typename T>
  object_result_impl_no_id<T>::
  ~object_result_impl_no_id ()
  {
  }

  template <typename T>
  typename object_result_impl_no_id<T>::pointer_type&
  object_result_impl_no_id<T>::
  current ()
  {
    if (pointer_traits::null_ptr (current_) && !end_)
    {
      // Objects without ids are not stored in session cache.
      //
      pointer_type p (object_traits::create ());
      object_type& obj (pointer_traits::get_ref (p));
      current (p);
      load (obj);
    }

    return current_;
  }

  //
  // object_result_iterator
  //

  template <typename T, typename ID>
  void object_result_iterator<T, ID>::
  load (object_type& obj)
  {
    if (res_->end ())
      return;

    if (!session::has_current ())
      res_->load (obj);
    else
    {
      typename reference_cache_traits<object_type>::insert_guard ig (
        reference_cache_traits<object_type>::insert (
          res_->database (), res_->load_id (), obj));
      res_->load (obj);
      ig.release ();
    }
  }
}
