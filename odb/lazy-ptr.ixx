// file      : odb/lazy-ptr.ixx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

namespace odb
{
  //
  // lazy_ptr
  //

  template <class T>
  inline lazy_ptr<T>::
  lazy_ptr (): p_ (0) {}

  template <class T>
  template <class Y>
  inline lazy_ptr<T>::
  lazy_ptr (Y* p): p_ (p) {}

  template <class T>
  inline lazy_ptr<T>::
  lazy_ptr (const lazy_ptr& r): p_ (r.p_), i_ (r.i_) {}

  template <class T>
  template <class Y>
  inline lazy_ptr<T>::
  lazy_ptr (const lazy_ptr<Y>& r): p_ (r.p_), i_ (r.i_) {}

  template <class T>
  inline lazy_ptr<T>& lazy_ptr<T>::
  operator= (const lazy_ptr& r)
  {
    p_ = r.p_;
    i_ = r.i_;
    return *this;
  }

  template <class T>
  template <class Y>
  inline lazy_ptr<T>& lazy_ptr<T>::
  operator= (Y* r)
  {
    p_ = r;
    i_.reset ();
    return *this;
  }

  template <class T>
  template <class Y>
  inline lazy_ptr<T>& lazy_ptr<T>::
  operator= (const lazy_ptr<Y>& r)
  {
    p_ = r.p_;
    i_ = r.i_;
    return *this;
  }

  template <class T>
  inline void lazy_ptr<T>::
  swap (lazy_ptr& b)
  {
    T* p (p_);
    p_ = b.p_;
    b.p_ = p;
    i_.swap (b.i_);
  }

  template <class T>
  inline void lazy_ptr<T>::
  reset ()
  {
    p_ = 0;
    i_.reset ();
  }

  template <class T>
  template <class Y>
  inline void lazy_ptr<T>::
  reset (Y* p)
  {
    p_ = p;
    i_.reset ();
  }

  template <class T>
  inline T& lazy_ptr<T>::
  operator* () const
  {
    return *p_;
  }

  template <class T>
  inline T* lazy_ptr<T>::
  operator-> () const
  {
    return p_;
  }

  template <class T>
  inline T* lazy_ptr<T>::
  get () const
  {
    return p_;
  }

  template <class T>
  inline bool lazy_ptr<T>::
  loaded () const
  {
    bool i (i_);
    return (p_ == 0) != i; // !p_ XOR i
  }

  template <class T>
  inline T* lazy_ptr<T>::
  load () const
  {
    if (!loaded ())
      p_ = i_.template load<T> (true); // Reset id.

    return p_;
  }

  template <class T>
  inline void lazy_ptr<T>::
  unload () const
  {
    typedef typename object_traits<T>::object_type object_type;

    if (p_)
    {
      if (i_.database () != 0)
        i_.reset_id (object_traits<object_type>::id (*p_));

      p_ = 0;
    }
  }

  template <class T>
  template <class ID>
  inline lazy_ptr<T>::
  lazy_ptr (database_type& db, const ID& id): p_ (0), i_ (db, id) {}

  template <class T>
  template <class Y>
  inline lazy_ptr<T>::
  lazy_ptr (database_type& db, Y* r)
      : p_ (r)
  {
    if (p_)
      i_.reset (db);
  }

  template <class T>
  template <class ID>
  inline void lazy_ptr<T>::
  reset (database_type& db, const ID& id)
  {
    p_ = 0;
    i_.reset (db, id);
  }

  template <class T>
  template <class Y>
  inline void lazy_ptr<T>::
  reset (database_type& db, Y* r)
  {
    p_ = r;

    if (p_)
      i_.reset (db);
    else
      i_.reset ();
  }

  template <class T>
  template <class O>
  inline typename object_traits<O>::id_type lazy_ptr<T>::
  object_id () const
  {
    typedef typename object_traits<T>::object_type object_type;

    return p_ ? object_traits<object_type>::id (*p_) : i_.object_id<O> ();
  }

  template <class T>
  inline typename lazy_ptr<T>::database_type& lazy_ptr<T>::
  database () const
  {
    return *i_.database ();
  }

  template<class T, class Y>
  inline bool
  operator== (const lazy_ptr<T>& a, const lazy_ptr<Y>& b)
  {
    return a.equal (b);
  }

  template<class T, class Y>
  inline bool
  operator!= (const lazy_ptr<T>& a, const lazy_ptr<Y>& b)
  {
    return !a.equal (b);
  }

  template<class T>
  inline void
  swap (lazy_ptr<T>& a, lazy_ptr<T>& b)
  {
    a.swap (b);
  }

  //
  // lazy_auto_ptr_ref
  //

  template<class T>
  inline lazy_auto_ptr_ref<T>::
  lazy_auto_ptr_ref (T* p, const lazy_ptr_impl_ref& i): p_ (p), i_ (i) {}

  //
  // lazy_auto_ptr
  //

  template<class T>
  inline lazy_auto_ptr<T>::
  lazy_auto_ptr (T* p): p_ (p) {}

  template<class T>
  inline lazy_auto_ptr<T>::
  lazy_auto_ptr (lazy_auto_ptr& r)
      : p_ (r.p_), i_ (static_cast<lazy_ptr_impl_ref> (r.i_))
  {
  }

  template<class T>
  template<class Y>
  inline lazy_auto_ptr<T>::
  lazy_auto_ptr (lazy_auto_ptr<Y>& r)
      : p_ (r.p_), i_ (static_cast<lazy_ptr_impl_ref> (r.i_))
  {
  }

  template<class T>
  inline lazy_auto_ptr<T>& lazy_auto_ptr<T>::
  operator= (lazy_auto_ptr& r)
  {
    p_ = r.p_;
    i_ = static_cast<lazy_ptr_impl_ref> (r.i_);
    return *this;
  }

  template<class T>
  template<class Y>
  inline lazy_auto_ptr<T>& lazy_auto_ptr<T>::
  operator= (lazy_auto_ptr<Y>& r)
  {
    p_ = r.p_;
    i_ = static_cast<lazy_ptr_impl_ref> (r.i_);
    return *this;
  }

  template<class T>
  inline T& lazy_auto_ptr<T>::
  operator* () const
  {
    return *p_;
  }

  template<class T>
  inline T* lazy_auto_ptr<T>::
  operator-> () const
  {
    return p_.operator-> ();
  }

  template<class T>
  inline T* lazy_auto_ptr<T>::
  get () const
  {
    return p_.get ();
  }

  template<class T>
  inline T* lazy_auto_ptr<T>::
  release ()
  {
    i_.reset ();
    return p_.release ();
  }

  template<class T>
  inline void lazy_auto_ptr<T>::
  reset (T* p)
  {
    i_.reset ();
    p_.reset (p);
  }

  template<class T>
  inline lazy_auto_ptr<T>::
  lazy_auto_ptr (const lazy_auto_ptr_ref<T>& r): p_ (r.p_), i_ (r.i_) {}

  template<class T>
  inline lazy_auto_ptr<T>& lazy_auto_ptr<T>::
  operator= (const lazy_auto_ptr_ref<T>& r)
  {
    if (p_.get () != r.p_)
      p_.reset (r.p_);

    i_ = r.i_;
    return *this;
  }

  template<class T>
  template<class Y>
  inline lazy_auto_ptr<T>::
  operator lazy_auto_ptr_ref<Y> ()
  {
    return lazy_auto_ptr_ref<Y> (p_.release (), i_);
  }

  template<class T>
  template<class Y>
  inline lazy_auto_ptr<T>::
  operator lazy_auto_ptr<Y> ()
  {
    return lazy_auto_ptr<Y> (*this);
  }

  template<class T>
  template <class Y>
  inline lazy_auto_ptr<T>::
  lazy_auto_ptr (std::auto_ptr<Y>& r): p_ (r) {}

  template<class T>
  inline lazy_auto_ptr<T>::
  lazy_auto_ptr (std::auto_ptr_ref<T> r): p_ (r) {}

  template<class T>
  template <class Y>
  inline lazy_auto_ptr<T>& lazy_auto_ptr<T>::
  operator= (std::auto_ptr<Y>& r)
  {
    p_ = r;
    i_.reset ();
    return *this;
  }

  template<class T>
  inline lazy_auto_ptr<T>& lazy_auto_ptr<T>::
  operator= (std::auto_ptr_ref<T> r)
  {
    p_ = r;
    i_.reset ();
    return *this;
  }

  template <class T>
  inline bool lazy_auto_ptr<T>::
  loaded () const
  {
    bool i (i_);
    return (p_.get () == 0) != i; // XOR
  }

  template <class T>
  inline std::auto_ptr<T>& lazy_auto_ptr<T>::
  load () const
  {
    if (!loaded ())
    {
      std::auto_ptr<T> tmp (i_.template load<T> (true)); // Reset id.
      p_ = tmp;
    }

    return p_;
  }

  template <class T>
  inline void lazy_auto_ptr<T>::
  unload () const
  {
    typedef typename object_traits<T>::object_type object_type;

    if (p_.get () != 0)
    {
      if (i_.database () != 0)
        i_.reset_id (object_traits<object_type>::id (*p_));

      p_.reset ();
    }
  }

  template <class T>
  template <class ID>
  inline lazy_auto_ptr<T>::
  lazy_auto_ptr (database_type& db, const ID& id): i_ (db, id) {}

  template <class T>
  inline lazy_auto_ptr<T>::
  lazy_auto_ptr (database_type& db, T* p)
      : p_ (p)
  {
    if (p)
      i_.reset (db);
  }

  template <class T>
  template <class Y>
  inline lazy_auto_ptr<T>::
  lazy_auto_ptr (database_type& db, std::auto_ptr<Y>& p)
      : p_ (p)
  {
    if (p)
      i_.reset (db);
  }

  template <class T>
  template <class ID>
  inline void lazy_auto_ptr<T>::
  reset (database_type& db, const ID& id)
  {
    p_.reset ();
    i_.reset (db, id);
  }

  template <class T>
  inline void lazy_auto_ptr<T>::
  reset (database_type& db, T* p)
  {
    p_.reset (p);

    if (p)
      i_.reset (db);
    else
      i_.reset ();
  }

  template <class T>
  template <class Y>
  inline void lazy_auto_ptr<T>::
  reset (database_type& db, std::auto_ptr<Y>& p)
  {
    p_ = p;

    if (p_.get () != 0)
      i_.reset (db);
    else
      i_.reset ();
  }

  template <class T>
  template <class O>
  inline typename object_traits<O>::id_type lazy_auto_ptr<T>::
  object_id () const
  {
    typedef typename object_traits<T>::object_type object_type;

    return p_.get () != 0
      ? object_traits<object_type>::id (*p_)
      : i_.object_id<O> ();
  }

  template <class T>
  inline typename lazy_auto_ptr<T>::database_type& lazy_auto_ptr<T>::
  database () const
  {
    return *i_.database ();
  }
}
