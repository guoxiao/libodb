// file      : odb/polymorphic-map.ixx
// copyright : Copyright (c) 2005-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

namespace odb
{
  template <typename T>
  inline polymorphic_entry<T>::
  polymorphic_entry ()
  {
    polymorphic_entry_impl<root_type>::insert (object_traits::info);
  }

  template <typename T>
    inline polymorphic_entry<T>::
  ~polymorphic_entry ()
  {
      polymorphic_entry_impl<root_type>::erase (object_traits::info);
  }
}
