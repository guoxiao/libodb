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
  class result_impl;

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
    result_iterator ()
        : res_ (0)
    {
    }

    explicit
    result_iterator (result_impl<T>& res)
        : res_ (&res)
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

    bool
    equal (const result_iterator& j)
    {
      return  (res_ ? res_->current (false) : 0) ==
        (j.res_ ? j.res_->current (false) : 0);
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
  class result_impl: public shared_base
  {
  public:
    virtual
    ~result_impl ();

  protected:
    friend class result<T>;
    friend class result_iterator<T>;

    virtual typename object_traits<T>::pointer_type
    current (bool release) = 0;

    virtual void
    next () = 0;
  };

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
    result (shared_ptr<result_impl<T> > impl)
        : impl_ (impl)
    {
    }

    /*
    result&
    operator= (shared_ptr<result_impl<T> > impl)
    {
      impl_ = impl;
    }
    */

  public:
    iterator
    begin ()
    {
      return iterator (*impl_);
    }

    iterator
    end ()
    {
      return iterator ();
    }

  public:
    operator shared_ptr<result_impl<T> > ()
    {
      return impl_;
    }

    // Copying or assignment of results is not supported.
    //
  private:
    result (const result&);
    result& operator= (const result&);

  private:
    shared_ptr<result_impl<T> > impl_;
  };
}

#include <odb/result.txx>

#endif // ODB_RESULT_HXX
