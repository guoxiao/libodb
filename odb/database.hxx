// file      : odb/database.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DATABASE_HXX
#define ODB_DATABASE_HXX

#include <odb/traits.hxx>
#include <odb/forward.hxx>
#include <odb/query.hxx>
#include <odb/result.hxx>

namespace odb
{
  class transaction_impl;

  class database
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
    load (typename object_traits<T>::id_type const& id);

    template <typename T>
    void
    load (typename object_traits<T>::id_type const& id, T& object);

    // Return NULL/false if not found.
    //
    template <typename T>
    typename object_traits<T>::pointer_type
    find (typename object_traits<T>::id_type const& id);

    template <typename T>
    bool
    find (typename object_traits<T>::id_type const& id, T& object);

    // Save the state of a modified objects.
    //
    template <typename T>
    void
    store (T& object);

    // Make the object transient. Throw object_not_persistent if not
    // found.
    //
    template <typename T>
    void
    erase (const T& object);

    template <typename T>
    void
    erase (typename object_traits<T>::id_type const& id);

    // Object query API.
    //
    template <typename T>
    result<T>
    query ();

    template <typename T>
    result<T>
    query (const std::string&);

    template <typename T>
    result<T>
    query (const odb::query<T>&);

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

#endif // ODB_DATABASE_HXX
