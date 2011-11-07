// file      : odb/database.ixx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cstring> // std::string

namespace odb
{
  inline database::
  database ()
      : tracer_ (0)
  {
  }

  inline connection_ptr database::
  connection ()
  {
    return connection_ptr (connection_ ());
  }

  inline void database::
  tracer (tracer_type& t)
  {
    tracer_ = &t;
  }

  inline void database::
  tracer (tracer_type* t)
  {
    tracer_ = t;
  }

  inline database::tracer_type* database::
  tracer () const
  {
    return tracer_;
  }

  template <typename T>
  inline typename object_traits<T>::id_type database::
  persist (T* p)
  {
    typedef typename object_traits<T>::pointer_type object_pointer;

    // The passed pointer should be the same or implicit-convertible
    // to the object pointer. This way we make sure the object pointer
    // does not assume ownership of the passed object.
    //
    const object_pointer& pobj (p);

    return persist_<T> (pobj);
  }

  template <typename T, template <typename> class P>
  inline typename object_traits<T>::id_type database::
  persist (const P<T>& p)
  {
    typedef typename object_traits<T>::pointer_type object_pointer;

    // The passed pointer should be the same or implicit-convertible
    // to the object pointer. This way we make sure the object pointer
    // does not assume ownership of the passed object.
    //
    const object_pointer& pobj (p);

    return persist_<T> (pobj);
  }

  template <typename T, template <typename> class P>
  inline typename object_traits<T>::id_type database::
  persist (P<T>& p)
  {
    const P<T>& cr (p);
    return persist<T, P> (cr);
  }

  template <typename T>
  inline typename object_traits<T>::id_type database::
  persist (const typename object_traits<T>::pointer_type& pobj)
  {
    return persist_<T> (pobj);
  }

  template <typename T>
  inline void database::
  reload (T* p)
  {
    reload<T> (*p);
  }

  template <typename T, template <typename> class P>
  inline void database::
  reload (const P<T>& p)
  {
    reload (odb::pointer_traits< P<T> >::get_ref (p));
  }

  template <typename T, template <typename> class P>
  inline void database::
  reload (P<T>& p)
  {
    reload (odb::pointer_traits< P<T> >::get_ref (p));
  }

  template <typename T>
  inline void database::
  reload (const typename object_traits<T>::pointer_type& pobj)
  {
    typedef typename object_traits<T>::pointer_type pointer_type;

    reload (odb::pointer_traits<pointer_type>::get_ref (pobj));
  }

  template <typename T>
  inline void database::
  update (T* p)
  {
    typedef typename object_traits<T>::pointer_type object_pointer;

    // The passed pointer should be the same or implicit-convertible
    // to the object pointer. This way we make sure the object pointer
    // does not assume ownership of the passed object.
    //
    const object_pointer& pobj (p);

    update_<T> (pobj);
  }

  template <typename T, template <typename> class P>
  inline void database::
  update (const P<T>& p)
  {
    typedef typename object_traits<T>::pointer_type object_pointer;

    // The passed pointer should be the same or implicit-convertible
    // to the object pointer. This way we make sure the object pointer
    // does not assume ownership of the passed object.
    //
    const object_pointer& pobj (p);

    update_<T> (pobj);
  }

  template <typename T, template <typename> class P>
  inline void database::
  update (P<T>& p)
  {
    const P<T>& cr (p);
    update<T, P> (cr);
  }

  template <typename T>
  inline void database::
  update (const typename object_traits<T>::pointer_type& pobj)
  {
    update_<T> (pobj);
  }

  template <typename T>
  inline void database::
  erase (T* p)
  {
    typedef typename object_traits<T>::pointer_type object_pointer;

    // The passed pointer should be the same or implicit-convertible
    // to the object pointer. This way we make sure the object pointer
    // does not assume ownership of the passed object.
    //
    const object_pointer& pobj (p);

    erase_<T> (pobj);
  }

  template <typename T, template <typename> class P>
  inline void database::
  erase (const P<T>& p)
  {
    typedef typename object_traits<T>::pointer_type object_pointer;

    // The passed pointer should be the same or implicit-convertible
    // to the object pointer. This way we make sure the object pointer
    // does not assume ownership of the passed object.
    //
    const object_pointer& pobj (p);

    erase_<T> (pobj);
  }

  template <typename T, template <typename> class P>
  inline void database::
  erase (P<T>& p)
  {
    const P<T>& cr (p);
    erase<T, P> (cr);
  }

  template <typename T>
  inline void database::
  erase (const typename object_traits<T>::pointer_type& pobj)
  {
    erase_<T> (pobj);
  }

  template <typename T>
  inline void database::
  erase_ (const typename object_traits<T>::pointer_type& pobj)
  {
    typedef typename object_traits<T>::pointer_type pointer_type;
    typedef pointer_traits<pointer_type> pointer_traits;

    erase<T> (pointer_traits::get_ref (pobj));
  }

  template <typename T>
  inline unsigned long long database::
  erase_query ()
  {
    // T is always object_type.
    //
    return erase_query<T> (odb::query<T> ());
  }

  template <typename T>
  inline unsigned long long database::
  erase_query (const char* q)
  {
    // T is always object_type.
    //
    return erase_query<T> (odb::query<T> (q));
  }

  template <typename T>
  inline unsigned long long database::
  erase_query (const std::string& q)
  {
    // T is always object_type.
    //
    return erase_query<T> (odb::query<T> (q));
  }

  template <typename T>
  inline result<T> database::
  query (bool cache)
  {
    return query<T> (odb::query<T> (), cache);
  }

  template <typename T>
  inline result<T> database::
  query (const char* q, bool cache)
  {
    return query<T> (odb::query<T> (q), cache);
  }

  template <typename T>
  inline result<T> database::
  query (const std::string& q, bool cache)
  {
    return query<T> (odb::query<T> (q), cache);
  }

  inline unsigned long long database::
  execute (const char* statement)
  {
    return execute (statement, std::strlen (statement));
  }

  inline unsigned long long database::
  execute (const std::string& statement)
  {
    return execute (statement.c_str (), statement.size ());
  }
}
