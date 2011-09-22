// file      : odb/view-result.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_VIEW_RESULT_HXX
#define ODB_VIEW_RESULT_HXX

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
  class result_impl<T, class_view>: public details::shared_base
  {
  public:
    virtual
    ~result_impl ();

  protected:
    friend class result<T, class_view>;
    friend class result<const T, class_view>;
    friend class result_iterator<T, class_view>;
    friend class result_iterator<const T, class_view>;

    typedef odb::database database_type;

    // In result_impl, T is always non-const and the same as view_type.
    //
    typedef T view_type;
    typedef odb::view_traits<view_type> view_traits;

    typedef typename view_traits::pointer_type pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    result_impl (database_type& db)
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
    load (view_type&) = 0;

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

  template <typename T>
  class result_iterator<T, class_view>
  {
  public:
    typedef T value_type;
    typedef value_type& reference;
    typedef value_type* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::input_iterator_tag iterator_category;

    // T can be const T while view_type is always non-const.
    //
    typedef typename view_traits<T>::view_type view_type;

    typedef result_impl<view_type, class_view> result_impl_type;

  public:
    explicit
    result_iterator (result_impl_type* res = 0)
        : res_ (res)
    {
    }

    // Input iterator requirements.
    //
  public:
    reference
    operator* () const
    {
      return pointer_traits::get_ref (res_->current ());
    }

    // Our value_type is already a pointer so return it instead of
    // a pointer to it (operator-> will just have to go one deeper
    // in the latter case).
    //
    pointer
    operator-> () const
    {
      return pointer_traits::get_ptr (res_->current ());
    }

    result_iterator&
    operator++ ()
    {
      res_->next ();
      return *this;
    }

    result_iterator
    operator++ (int)
    {
      // All non-end iterators for a result object move together.
      //
      res_->next ();
      return *this;
    }

  public:
    typename view_traits<T>::pointer_type
    load ()
    {
      typename view_traits<T>::pointer_type r (res_->current ());
      res_->release ();
      return r;
    }

    void
    load (view_type&);

  public:
    bool
    equal (result_iterator j) const
    {
      return  (res_ ? res_->end () : true) == (j.res_ ? j.res_->end () : true);
    }

  private:
    // Use unrestricted pointer traits since that's what is returned by
    // result_impl.
    //
    typedef
    odb::pointer_traits<typename view_traits<view_type>::pointer_type>
    pointer_traits;

    result_impl_type* res_;
  };

  // Input iterator requirements.
  //
  template <typename T>
  inline bool
  operator== (result_iterator<T, class_view> i,
              result_iterator<T, class_view> j)
  {
    return i.equal (j);
  }

  template <typename T>
  inline bool
  operator!= (result_iterator<T, class_view> i,
              result_iterator<T, class_view> j)
  {
    return !i.equal (j);
  }

  //
  //
  template <typename T>
  class result<T, class_view>
  {
  public:
    typedef typename view_traits<T>::pointer_type value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;

    typedef result_iterator<T, class_view> iterator;

    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

    // T can be const T while view_type is always non-const.
    //
    typedef typename view_traits<T>::view_type view_type;
    typedef result_impl<view_type, class_view> result_impl_type;

  public:
    result ()
    {
    }

    explicit
    result (details::shared_ptr<result_impl_type> impl)
        : impl_ (impl)
    {
    }

    // Copying or assignment of a result object leads to one instance
    // being an alias for another. Think of copying a result as copying
    // a file handle -- the file you access through either of them is
    // still the same.
    //
  public:
    result (const result& r)
        : impl_ (r.impl_)
    {
    }

    result&
    operator= (const result& r)
    {
      if (impl_ != r.impl_)
        impl_ = r.impl_;

      return *this;
    }

    // Conversion from result<T> to result<const T>.
    //
    template <typename UT>
    result (const result<UT, class_view>& r)
        //
        // If you get a compiler error pointing to the line below saying
        // that the impl_ member is inaccessible, then you are most likely
        // trying to perform an illegal result conversion, for example,
        // from result<const obj> to result<obj>.
        //
        : impl_ (r.impl_)
    {
    }

    template <typename UT>
    result&
    operator= (const result<UT, class_view>& r)
    {
      // If you get a compiler error pointing to the line below saying
      // that the impl_ member is inaccessible, then you are most likely
      // trying to perform an illegal result conversion, for example,
      // from result<const obj> to result<obj>.
      //
      if (impl_ != r.impl_)
        impl_ = r.impl_;

      return *this;
    }

    void
    swap (result& r)
    {
      // @@ add swap() to shared_ptr.
      //
      details::shared_ptr<result_impl_type> p (impl_);
      impl_ = r.impl_;
      r.impl_ = p;
    }

  public:
    iterator
    begin ()
    {
      if (impl_)
        impl_->begin ();

      return iterator (impl_.get ());
    }

    iterator
    end ()
    {
      return iterator ();
    }

    // Cache the result instead of fetching the data from the database
    // one view at a time. This is necessary if you plan on performing
    // database operations while iterating over the result.
    //
  public:
    void
    cache ()
    {
      if (impl_)
        impl_->cache ();
    }

  public:
    bool
    empty () const
    {
      if (impl_ == 0)
        return true;

      impl_->begin ();
      return impl_->end ();
    }

    // Size is only known in cached results.
    //
    size_type
    size () const
    {
      return impl_ ? impl_->size () : 0;
    }

  private:
    friend class result<const T>;

    details::shared_ptr<result_impl_type> impl_;
  };
}

#include <odb/view-result.txx>

#include <odb/post.hxx>

#endif // ODB_VIEW_RESULT_HXX
