// file      : odb/lazy-ptr.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_LAZY_PTR_HXX
#define ODB_LAZY_PTR_HXX

#include <odb/pre.hxx>

#include <memory> // std::auto_ptr

#include <odb/forward.hxx> // odb::database
#include <odb/traits.hxx>
#include <odb/lazy-ptr-impl.hxx>

namespace odb
{
  template <class T>
  class lazy_ptr
  {
    // Pointer interface.
    //
  public:
    typedef T element_type;

    lazy_ptr ();
    template <class Y> lazy_ptr (Y*);

    lazy_ptr (const lazy_ptr&);
    template <class Y> lazy_ptr (const lazy_ptr<Y>&);

    lazy_ptr& operator= (const lazy_ptr&);
    template <class Y> lazy_ptr& operator= (Y*);
    template <class Y> lazy_ptr& operator= (const lazy_ptr<Y>&);

    void swap (lazy_ptr&);
    void reset ();
    template <class Y> void reset (Y*);

    T& operator* () const;
    T* operator-> () const;
    T* get () const;

    typedef T* lazy_ptr::*unspecified_bool_type;
    operator unspecified_bool_type () const
    {
      return (p_ || i_) ? &lazy_ptr::p_ : 0;
    }

    // Lazy loading interface.
    //
  public:
    typedef odb::database database_type;

    //  NULL      loaded()
    //
    //  true       true      NULL pointer to transient object
    //  false      true      valid pointer to persistent object
    //  true       false     unloaded pointer to persistent object
    //  false      false     valid pointer to transient object
    //
    bool loaded () const;

    T* load () const;

    // Unload the pointer. For transient objects this function is
    // equivalent to reset().
    //
    void unload () const;

    template <class ID> lazy_ptr (database_type&, const ID&);
    template <class Y> lazy_ptr (database_type&, Y*);

    template <class ID> void reset (database_type&, const ID&);
    template <class Y> void reset (database_type&, Y*);

    template <class O /* = T */>
    typename object_traits<O>::id_type object_id () const;

    database_type& database () const;

    // Helpers.
    //
  public:
    template <class Y> bool equal (const lazy_ptr<Y>&) const;

  private:
    template <class Y> friend class lazy_ptr;

    mutable T* p_;
    mutable lazy_ptr_impl<T> i_;
  };

  // operator< and operator<< are not provided.
  //
  template<class T, class Y>
  bool operator== (const lazy_ptr<T>&, const lazy_ptr<Y>&);

  template<class T, class Y>
  bool operator!= (const lazy_ptr<T>&, const lazy_ptr<Y>&);

  template<class T> void swap (lazy_ptr<T>&, lazy_ptr<T>&);

  //
  //
  template <class T>
  struct lazy_auto_ptr_ref
  {
    explicit lazy_auto_ptr_ref (T*, const lazy_ptr_impl_ref&);

    T* p_;
    lazy_ptr_impl_ref i_;
  };

  template <class T>
  class lazy_auto_ptr
  {
    // Standard auto_ptr interface.
    //
  public:
    typedef T element_type;

    explicit lazy_auto_ptr (T* = 0);
    lazy_auto_ptr (lazy_auto_ptr&);
    template<class Y> lazy_auto_ptr (lazy_auto_ptr<Y>&);

    lazy_auto_ptr& operator= (lazy_auto_ptr&);
    template<class Y> lazy_auto_ptr& operator= (lazy_auto_ptr<Y>&);

    T& operator* () const;
    T* operator-> () const;
    T* get () const;
    T* release ();
    void reset (T* = 0);

    lazy_auto_ptr (const lazy_auto_ptr_ref<T>&);
    lazy_auto_ptr& operator= (const lazy_auto_ptr_ref<T>&);
    template<class Y> operator lazy_auto_ptr_ref<Y> ();
    template<class Y> operator lazy_auto_ptr<Y> ();

    // Extension: conversion to bool.
    //
  public:
    typedef std::auto_ptr<T> lazy_auto_ptr::*unspecified_bool_type;
    operator unspecified_bool_type () const
    {
      return (p_.get () != 0 || i_) ? &lazy_auto_ptr::p_ : 0;
    }

    // Initialization/assignment from auto_ptr.
    //
  public:
    template <class Y> lazy_auto_ptr (std::auto_ptr<Y>&);
    lazy_auto_ptr (std::auto_ptr_ref<T>);

    template <class Y> lazy_auto_ptr& operator= (std::auto_ptr<Y>&);
    lazy_auto_ptr& operator= (std::auto_ptr_ref<T>);

    // Lazy loading interface.
    //
  public:
    typedef odb::database database_type;

    //  NULL      loaded()
    //
    //  true       true      NULL pointer to transient object
    //  false      true      valid pointer to persistent object
    //  true       false     unloaded pointer to persistent object
    //  false      false     valid pointer to transient object
    //
    bool loaded () const;

    std::auto_ptr<T>& load () const;

    // Unload the pointer. For transient objects this function is
    // equivalent to reset().
    //
    void unload () const;

    template <class ID> lazy_auto_ptr (database_type&, const ID&);
    lazy_auto_ptr (database_type&, T*);
    template <class Y> lazy_auto_ptr (database_type&, std::auto_ptr<Y>&);

    template <class ID> void reset (database_type&, const ID&);
    void reset (database_type&, T*);
    template <class Y> void reset (database_type&, std::auto_ptr<Y>&);

    template <class O /* = T */>
    typename object_traits<O>::id_type object_id () const;

    database_type& database () const;

  private:
    template <class Y> friend class lazy_auto_ptr;

    // Note that it is possible to have a situation where p_ is NULL,
    // i_.id is NULL and i_.db is not NULL. This will happen if the
    // auto_ptr reference returned by load() is transferred to another
    // pointer or reset.
    //
    mutable std::auto_ptr<T> p_;
    mutable lazy_ptr_impl<T> i_;
  };

  namespace core
  {
    using odb::lazy_ptr;
    using odb::lazy_auto_ptr;
  }
}

#include <odb/lazy-ptr.ixx>
#include <odb/lazy-ptr.txx>

#include <odb/lazy-pointer-traits.hxx>

#include <odb/post.hxx>

#endif // ODB_LAZY_PTR_HXX
