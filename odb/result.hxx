// file      : odb/result.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_RESULT_HXX
#define ODB_RESULT_HXX

#include <cstddef>  // std::ptrdiff_t, std::size_t
#include <iterator> // iterator categories

#include <odb/forward.hxx>
#include <odb/shared-ptr.hxx>

namespace odb
{
  template <typename T>
  class result;

  template <typename T>
  class result_iterator;

  template <typename T>
  class result_impl: public shared_base
  {
  public:
    virtual
    ~result_impl ();
    result_impl () : end_ (false), current_ () {}

  protected:
    friend class result<T>;
    friend class result_iterator<T>;

    typename object_traits<T>::pointer_type
    current (bool /*release*/)
    {
      if (object_traits<T>::pointer_ops::null_ptr (current_) && !end_)
        current ();

      return current_;
    }

    bool
    end () const
    {
      return end_;
    }

  protected:
    virtual void
    current () = 0;

    virtual void
    current (T&) = 0;

    virtual void
    next () = 0;

  protected:
    bool end_;
    typename object_traits<T>::pointer_type current_;
  };

  template <typename T>
  class result_iterator
  {
  public:
    typedef typename object_traits<T>::pointer_type value_type;
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
    value_type
    operator* () const
    {
      return res_->current (true);
    }

    // Our value_type is already a pointer so return it instead of
    // a pointer to it (operator-> will just have to go one deeper
    // in the latter case).
    //
    value_type
    operator-> () const
    {
      return res_->current (false);
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
    result (shared_ptr<result_impl<T> > impl)
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

  private:
    shared_ptr<result_impl<T> > impl_;
  };
}

#include <odb/result.txx>

#endif // ODB_RESULT_HXX
