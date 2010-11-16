// file      : odb/result.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_RESULT_HXX
#define ODB_RESULT_HXX

#include <odb/pre.hxx>

#include <cstddef>  // std::ptrdiff_t, std::size_t
#include <iterator> // iterator categories

#include <odb/forward.hxx>
#include <odb/pointer-traits.hxx>

#include <odb/details/shared-ptr.hxx>

namespace odb
{
  template <typename T>
  class result;

  template <typename T>
  class result_iterator;

  template <typename T>
  class result_impl: public details::shared_base
  {
  public:
    virtual
    ~result_impl ();
    result_impl () : end_ (false), current_ () {}

  protected:
    friend class result<T>;
    friend class result_iterator<T>;

    typedef object_traits<T> traits;
    typedef typename traits::pointer_type pointer_type;
    typedef typename traits::pointer_traits pointer_traits;

    // To make this work with all kinds of pointers (naked, std::auto_ptr,
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

    bool
    end () const
    {
      return end_;
    }

  protected:
    virtual void
    current (T&) = 0;

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

    bool end_;

  private:
    pointer_type current_;
    typename pointer_traits::guard_type guard_;
  };

  template <typename T>
  class result_iterator
  {
  public:
    typedef T value_type;
    typedef value_type& reference;
    typedef value_type* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::input_iterator_tag iterator_category;

  public:
    explicit
    result_iterator (result_impl<T>* res = 0)
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
    typename object_traits<T>::pointer_type
    load ()
    {
      typename object_traits<T>::pointer_type r (res_->current ());
      res_->release ();
      return r;
    }

    void
    load (T& x)
    {
      res_->current (x);
    }

  public:
    bool
    equal (result_iterator j) const
    {
      return  (res_ ? res_->end () : true) == (j.res_ ? j.res_->end () : true);
    }

  private:
    typedef typename object_traits<T>::pointer_traits pointer_traits;

    result_impl<T>* res_;
  };

  // Input iterator requirements.
  //
  template <typename T>
  inline bool
  operator== (result_iterator<T> i, result_iterator<T> j)
  {
    return i.equal (j);
  }

  template <typename T>
  inline bool
  operator!= (result_iterator<T> i, result_iterator<T> j)
  {
    return !i.equal (j);
  }

  //
  //
  template <typename T>
  class result
  {
  public:
    typedef typename object_traits<T>::pointer_type value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;

    typedef result_iterator<T> iterator;

    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

  public:
    result ()
    {
    }

    explicit
    result (details::shared_ptr<result_impl<T> > impl)
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

    void
    swap (result& r)
    {
      // @@ add swap() to shared_ptr.
      //
      details::shared_ptr<result_impl<T> > p (impl_);
      impl_ = r.impl_;
      r.impl_ = p;
    }

  public:
    iterator
    begin ()
    {
      return iterator (impl_.get ());
    }

    iterator
    end ()
    {
      return iterator ();
    }

    // Cache the result instead of fetching the data from the database
    // one object at a time. This is necessary if you plan on performing
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
      return impl_ == 0 || impl_->end ();
    }

    // Size is only known in cached results.
    //
    size_type
    size () const
    {
      return impl_ ? impl_->size () : 0;
    }

  private:
    details::shared_ptr<result_impl<T> > impl_;
  };
}

#include <odb/result.txx>

#include <odb/post.hxx>

#endif // ODB_RESULT_HXX
