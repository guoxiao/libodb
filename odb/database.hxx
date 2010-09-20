// file      : odb/database.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DATABASE_HXX
#define ODB_DATABASE_HXX

#include <odb/pre.hxx>

#include <string>

#include <odb/traits.hxx>
#include <odb/forward.hxx>
#include <odb/query.hxx>
#include <odb/result.hxx>

#include <odb/details/export.hxx>

namespace odb
{
  class transaction_impl;

  class LIBODB_EXPORT database
  {
  public:
    virtual
    ~database ();

    // Object persistence API.
    //
  public:

    // Make the object persistent.
    //
    template <typename T>
    typename object_traits<T>::id_type
    persist (T& object);

    // Throw object_not_persistent if not found.
    //
    template <typename T>
    typename object_traits<T>::pointer_type
    load (const typename object_traits<T>::id_type& id);

    template <typename T>
    void
    load (const typename object_traits<T>::id_type& id, T& object);

    // Return NULL/false if not found.
    //
    template <typename T>
    typename object_traits<T>::pointer_type
    find (const typename object_traits<T>::id_type& id);

    template <typename T>
    bool
    find (const typename object_traits<T>::id_type& id, T& object);

    // Update the state of a modified objects.
    //
    template <typename T>
    void
    update (T& object);

    // Make the object transient. Throw object_not_persistent if not
    // found.
    //
    template <typename T>
    void
    erase (const T& object);

    template <typename T>
    void
    erase (const typename object_traits<T>::id_type& id);

    // Query API.
    //
    template <typename T>
    result<T>
    query (bool cache = true);

    template <typename T>
    result<T>
    query (const char*, bool cache = true);

    template <typename T>
    result<T>
    query (const std::string&, bool cache = true);

    template <typename T>
    result<T>
    query (const odb::query<T>&, bool cache = true);

    // Transaction API.
    //
  public:
    virtual transaction_impl*
    begin_transaction () = 0;

  protected:
    database ();

  private:
    database (const database&);
    database& operator= (const database&);
  };
}

#include <odb/database.ixx>
#include <odb/database.txx>

#include <odb/post.hxx>

#endif // ODB_DATABASE_HXX
