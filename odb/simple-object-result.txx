// file      : odb/simple-object-result.txx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

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
  void object_result_impl<T>::
  load ()
  {
    // First check the session.
    //
    const id_type& id (load_id ());

    pointer_type p (
      object_traits::pointer_cache_traits::find (database (), id));

    if (!pointer_traits::null_ptr (p))
      current (p, false); // Pointer from cache should not be guarded.
    else
    {
      p = object_traits::create ();

      typename object_traits::pointer_cache_traits::insert_guard ig (
        object_traits::pointer_cache_traits::insert (database (), id, p));

      object_type& obj (pointer_traits::get_ref (p));
      current (p);
      load (obj, false);
      ig.release ();
    }
  }

  //
  // object_result_iterator
  //

  template <typename T, typename ID>
  void object_result_iterator<T, ID, false>::
  load (object_type& obj)
  {
    if (res_->end ())
      return;

    typedef odb::object_traits<object_type> object_traits;

    typename object_traits::reference_cache_traits::insert_guard ig (
      object_traits::reference_cache_traits::insert (
        res_->database (), res_->load_id (), obj));
    res_->load (obj, false);
    ig.release ();
  }
}
