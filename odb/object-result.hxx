// file      : odb/object-result.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_OBJECT_RESULT_HXX
#define ODB_OBJECT_RESULT_HXX

#include <odb/pre.hxx>

#include <cstddef>  // std::ptrdiff_t, std::size_t
#include <iterator> // iterator categories

#include <odb/forward.hxx>
#include <odb/result.hxx>
#include <odb/pointer-traits.hxx>

#include <odb/details/shared-ptr.hxx>

namespace odb
{
  template <typename T>
  class object_result_impl;

  template <typename T>
  class object_result_impl_no_id;

  template <typename T, typename ID>
  class object_result_iterator;

  template <typename T, typename ID = typename object_traits<T>::id_type>
  struct object_result_impl_selector
  {
    typedef object_result_impl<T> type;
  };

  template <typename T>
  struct object_result_impl_selector<T, void>
  {
    typedef object_result_impl_no_id<T> type;
  };

  // Implementation for objects with object id.
  //
  template <typename T>
  class object_result_impl: public details::shared_base
  {
  public:
    virtual
    ~object_result_impl ();

  protected:
    typedef odb::database database_type;

    // In result_impl, T is always non-const and the same as object_type.
    //
    typedef T object_type;
    typedef odb::object_traits<object_type> object_traits;
    typedef typename object_traits::id_type id_type;

    typedef typename object_traits::pointer_type pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    friend class result<T>;
    friend class result<const T>;
    friend class result_iterator<T, class_object>;
    friend class result_iterator<const T, class_object>;
    friend class object_result_iterator<T, id_type>;
    friend class object_result_iterator<const T, id_type>;

  protected:
    object_result_impl (database_type& db)
        : begin_ (true), end_ (false), db_ (db), current_ ()
    {
    }

    database_type&
    database () const
    {
      return db_;
    }

    // To make this work with all kinds of pointers (raw, std::auto_ptr,
    // shared), we need to make sure we don't make any copies of the
    // pointer on the return path.
    //
    pointer_type&
    current ();

    void
    release ()
    {
      current_ = pointer_type ();
      guard_.release ();
    }

    void
    begin ()
    {
      if (begin_)
      {
        next ();
        begin_ = false;
      }
    }

    bool
    end () const
    {
      return end_;
    }

  protected:
    // The fetch argument is a hint to the implementation. If it is
    // false then it means load_id() was already called (and presumably
    // fetched the data into the object image) and the object image
    // is still valid (so the implementation doesn't need to fetch
    // the data again).
    //
    virtual void
    load (object_type&, bool fetch = true) = 0;

    virtual id_type
    load_id () = 0;

    virtual void
    next () = 0;

    virtual void
    cache () = 0;

    virtual std::size_t
    size () = 0;

  protected:
    void
    current (pointer_type p)
    {
      current_ = p;
      guard_.reset (current_);
    }

    bool begin_;
    bool end_;

  private:
    database_type& db_;
    pointer_type current_;
    typename pointer_traits::guard guard_;
  };

  // Implementation for objects without object id.
  //
  template <typename T>
  class object_result_impl_no_id: public details::shared_base
  {
  public:
    virtual
    ~object_result_impl_no_id ();

  protected:
    typedef odb::database database_type;

    // In result_impl, T is always non-const and the same as object_type.
    //
    typedef T object_type;
    typedef odb::object_traits<object_type> object_traits;

    typedef typename object_traits::pointer_type pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    friend class result<T>;
    friend class result<const T>;
    friend class result_iterator<T, class_object>;
    friend class result_iterator<const T, class_object>;
    friend class object_result_iterator<T, void>;
    friend class object_result_iterator<const T, void>;

  protected:
    object_result_impl_no_id (database_type& db)
        : begin_ (true), end_ (false), db_ (db), current_ ()
    {
    }

    database_type&
    database () const
    {
      return db_;
    }

    // To make this work with all kinds of pointers (raw, std::auto_ptr,
    // shared), we need to make sure we don't make any copies of the
    // pointer on the return path.
    //
    pointer_type&
    current ();

    void
    release ()
    {
      current_ = pointer_type ();
      guard_.release ();
    }

    void
    begin ()
    {
      if (begin_)
      {
        next ();
        begin_ = false;
      }
    }

    bool
    end () const
    {
      return end_;
    }

  protected:
    virtual void
    load (object_type&) = 0;

    virtual void
    next () = 0;

    virtual void
    cache () = 0;

    virtual std::size_t
    size () = 0;

  protected:
    void
    current (pointer_type p)
    {
      current_ = p;
      guard_.reset (current_);
    }

    bool begin_;
    bool end_;

  private:
    database_type& db_;
    pointer_type current_;
    typename pointer_traits::guard guard_;
  };

  //
  // result_iterator
  //

  template <typename T, typename ID>
  class object_result_iterator
  {
  public:
    // T can be const T while object_type is always non-const.
    //
    typedef typename object_traits<T>::object_type object_type;
    typedef typename object_traits<T>::id_type id_type;

    typedef object_result_impl<object_type> result_impl_type;

  public:
    object_result_iterator (result_impl_type* res)
        : res_ (res)
    {
    }

  public:
    typename object_traits<T>::pointer_type
    load ()
    {
      typename object_traits<T>::pointer_type r (res_->current ());
      res_->release ();
      return r;
    }

    void
    load (object_type&);

    id_type
    id ()
    {
      return res_->load_id ();
    }

  protected:
    result_impl_type* res_;
  };

  template <typename T>
  class object_result_iterator<T, void>
  {
  public:
    // T can be const T while object_type is always non-const.
    //
    typedef typename object_traits<T>::object_type object_type;

    typedef object_result_impl_no_id<object_type> result_impl_type;

  public:
    object_result_iterator (result_impl_type* res)
        : res_ (res)
    {
    }

  public:
    typename object_traits<T>::pointer_type
    load ()
    {
      typename object_traits<T>::pointer_type r (res_->current ());
      res_->release ();
      return r;
    }

    void
    load (object_type& obj)
    {
      // Objects without ids are not stored in session cache.
      //
      if (!res_->end ())
        res_->load (obj);
    }

  protected:
    result_impl_type* res_;
  };

  template <typename T>
  class result_iterator<T, class_object>: public object_result_iterator<
    T,
    typename object_traits<T>::id_type>
  {
  public:
    typedef T value_type;
    typedef value_type& reference;
    typedef value_type* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::input_iterator_tag iterator_category;

    // T can be const T while object_type is always non-const.
    //
    typedef
    object_result_iterator<T, typename object_traits<T>::id_type>
    base_type;

  public:
    explicit
    result_iterator (typename base_type::result_impl_type* res = 0)
        : base_type (res)
    {
    }

    // Input iterator requirements.
    //
  public:
    reference
    operator* () const
    {
      return pointer_traits::get_ref (this->res_->current ());
    }

    // Our value_type is already a pointer so return it instead of
    // a pointer to it (operator-> will just have to go one deeper
    // in the latter case).
    //
    pointer
    operator-> () const
    {
      return pointer_traits::get_ptr (this->res_->current ());
    }

    result_iterator&
    operator++ ()
    {
      this->res_->next ();
      return *this;
    }

    result_iterator
    operator++ (int)
    {
      // All non-end iterators for a result object move together.
      //
      this->res_->next ();
      return *this;
    }

  public:
    bool
    equal (result_iterator j) const
    {
      return  (this->res_ ? this->res_->end () : true) ==
        (j.res_ ? j.res_->end () : true);
    }

  private:
    // Use unrestricted pointer traits since that's what is returned by
    // result_impl.
    //
    typedef
    odb::pointer_traits<
      typename object_traits<
        typename base_type::object_type>::pointer_type>
    pointer_traits;
  };

  //
  //
  template <typename T>
  class result_base<T, class_object>
  {
  public:
    typedef typename object_traits<T>::pointer_type value_type;

    // T can be const T while object_type is always non-const.
    //
    typedef typename object_traits<T>::object_type object_type;
    typedef
    typename object_result_impl_selector<object_type>::type
    result_impl_type;
  };
}

#include <odb/object-result.txx>

#include <odb/post.hxx>

#endif // ODB_OBJECT_RESULT_HXX
