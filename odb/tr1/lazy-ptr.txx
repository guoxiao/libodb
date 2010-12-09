// file      : odb/tr1/lazy-ptr.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

namespace odb
{
  namespace tr1
  {
    //
    // lazy_shared_ptr
    //

    template <class T>
    template <class Y>
    bool lazy_shared_ptr<T>::
    equal (const lazy_shared_ptr<Y>& r) const
    {
      if (loaded () && r.loaded ())
        return p_ == r.p_;

      // If one of the object is not loaded, then we compare databases and
      // object ids. Note that NULL pointers cannot have non-NULL databases
      // and if both of them are NULL, we wouldn't have gotten here.
      //
      typedef typename object_traits<T>::object_type object_type1;
      typedef typename object_traits<Y>::object_type object_type2;

      return i_.database () == r.i_.database () &&
        object_id<object_type1> () == r.object_id<object_type2> ();
    }

    //
    // lazy_weak_ptr
    //

    template <class T>
    lazy_shared_ptr<T> lazy_weak_ptr<T>::
    lock () const
    {
      std::tr1::shared_ptr<T> sp (p_.lock ());

      if (sp)
      {
        if (database_type* db = i_.database ())
          return lazy_shared_ptr<T> (*db, sp);
        else
          return lazy_shared_ptr<T> (sp);
      }
      else
      {
        if (i_)
          return lazy_shared_ptr<T> (*i_.database (), i_.object_id<T> ());
        else
          return lazy_shared_ptr<T> ();
      }
    }
  }
}
