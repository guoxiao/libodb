// file      : odb/result.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/session.hxx>
#include <odb/cache-traits.hxx>

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
    typedef typename object_traits::pointer_type unrestricted_pointer_type;
    typedef typename object_traits::pointer_traits unrestricted_pointer_traits;

    if (pointer_traits::null_ptr (current_) && !end_)
    {
      if (!session::has_current ())
      {
        unrestricted_pointer_type up (object_traits::create ());
        object_type& obj (unrestricted_pointer_traits::get_ref (up));
        current (pointer_type (up));
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
          unrestricted_pointer_type up (object_traits::create ());

          typename
            pointer_cache_traits<unrestricted_pointer_type>::insert_guard ig (
              pointer_cache_traits<unrestricted_pointer_type>::insert (
                database (), id, up));

          object_type& obj (unrestricted_pointer_traits::get_ref (up));
          current (pointer_type (up));
          load (obj);
          ig.release ();
        }
      }
    }

    return current_;
  }

  //
  // result_iterator
  //

  template <typename T>
  void result_iterator<T>::
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
