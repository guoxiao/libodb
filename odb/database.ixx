// file      : odb/database.ixx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cstring> // std::strlen()

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

  template <typename T, typename A1, template <typename, typename> class P>
  inline typename object_traits<T>::id_type database::
  persist (const P<T, A1>& p)
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

  template <typename T, typename A1, template <typename, typename> class P>
  inline typename object_traits<T>::id_type database::
  persist (P<T, A1>& p)
  {
    const P<T, A1>& cr (p);
    return persist<T, A1, P> (cr);
  }

  template <typename T>
  inline typename object_traits<T>::id_type database::
  persist (const typename object_traits<T>::pointer_type& pobj)
  {
    return persist_<T> (pobj);
  }

  template <typename T>
  inline typename object_traits<T>::pointer_type database::
  find (const typename object_traits<T>::id_type& id)
  {
    // T is always object_type.
    //

    // Compiler error pointing here? Perhaps the object doesn't have the
    // default constructor?
    //
    return object_traits<T>::find (*this, id);
  }

  template <typename T>
  inline bool database::
  find (const typename object_traits<T>::id_type& id, T& obj)
  {
    // T is always object_type.
    //
    return object_traits<T>::find (*this, id, obj);
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

  template <typename T, typename A1, template <typename, typename> class P>
  inline void database::
  reload (const P<T, A1>& p)
  {
    reload (odb::pointer_traits< P<T, A1> >::get_ref (p));
  }

  template <typename T, template <typename> class P>
  inline void database::
  reload (P<T>& p)
  {
    reload (odb::pointer_traits< P<T> >::get_ref (p));
  }

  template <typename T, typename A1, template <typename, typename> class P>
  inline void database::
  reload (P<T, A1>& p)
  {
    reload (odb::pointer_traits< P<T, A1> >::get_ref (p));
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

  template <typename T, typename A1, template <typename, typename> class P>
  inline void database::
  update (const P<T, A1>& p)
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

  template <typename T, typename A1, template <typename, typename> class P>
  inline void database::
  update (P<T, A1>& p)
  {
    const P<T, A1>& cr (p);
    update<T, A1, P> (cr);
  }

  template <typename T>
  inline void database::
  update (const typename object_traits<T>::pointer_type& pobj)
  {
    update_<T> (pobj);
  }

  template <typename T>
  inline void database::
  update (T& obj)
  {
    // T can be const T while object_type will always be T.
    //
    typedef typename odb::object_traits<T>::object_type object_type;
    typedef odb::object_traits<object_type> object_traits;

    // Compiler error pointing here? Perhaps the object is readonly or
    // doesn't have an object id? Such objects cannot be updated.
    //
    object_traits::update (*this, obj);
  }

  template <typename T>
  inline void database::
  update_ (const typename object_traits<T>::pointer_type& pobj)
  {
    // T can be const T while object_type will always be T.
    //
    typedef typename odb::object_traits<T>::object_type object_type;
    typedef odb::object_traits<object_type> object_traits;

    typedef typename odb::object_traits<T>::pointer_type pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    T& obj (pointer_traits::get_ref (pobj));

    // Compiler error pointing here? Perhaps the object is readonly or
    // doesn't have an object id? Such objects cannot be updated.
    //
    object_traits::update (*this, obj);
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

  template <typename T, typename A1, template <typename, typename> class P>
  inline void database::
  erase (const P<T, A1>& p)
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

  template <typename T, typename A1, template <typename, typename> class P>
  inline void database::
  erase (P<T, A1>& p)
  {
    const P<T, A1>& cr (p);
    erase<T, A1, P> (cr);
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
  inline void database::
  erase (const typename object_traits<T>::id_type& id)
  {
    // T is always object_type.
    //
    object_traits<T>::erase (*this, id);
  }

  template <typename T>
  inline void database::
  erase (T& obj)
  {
    // T can be const T while object_type will always be T.
    //
    typedef typename object_traits<T>::object_type object_type;

    object_traits<object_type>::erase (*this, obj);
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
  inline unsigned long long database::
  erase_query (const odb::query<T>& q)
  {
    // T is always object_type.
    //
    return object_traits<T>::erase_query (*this, q);
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
