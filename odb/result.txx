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
        // Non-const pointer.
        //
        unrestricted_pointer_type p (object_traits::create ());
        object_type& r (unrestricted_pointer_traits::get_ref (p));

        current (pointer_type (p));
        current (r);
      }
      else
      {
        const id_type& id (current_id ());

        // First check the session.
        //
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

          object_type& r (unrestricted_pointer_traits::get_ref (up));

          current (pointer_type (up));
          current (r);

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
  load (object_type& x)
  {
    if (res_->end ())
      return;

    if (!session::has_current ())
      res_->current (x);
    else
    {
      const id_type& id (res_->current_id ());

      typename reference_cache_traits<object_type>::insert_guard ig (
        reference_cache_traits<object_type>::insert (
          res_->database (), id, x));

      res_->current (x);
      ig.release ();
    }
  }
}
