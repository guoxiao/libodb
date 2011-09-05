// file      : odb/database.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DATABASE_HXX
#define ODB_DATABASE_HXX

#include <odb/pre.hxx>

#include <string>
#include <cstddef> // std::size_t

#include <odb/traits.hxx>
#include <odb/forward.hxx>
#include <odb/query.hxx>
#include <odb/result.hxx>
#include <odb/connection.hxx>
#include <odb/exceptions.hxx>

#include <odb/details/export.hxx>
#include <odb/details/meta/remove-const.hxx>

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

    template <typename T>
    typename object_traits<T>::id_type
    persist (T* obj_ptr);

    template <typename T, template <typename> class P>
    typename object_traits<T>::id_type
    persist (const P<T>& obj_ptr);

    template <typename T, template <typename> class P>
    typename object_traits<T>::id_type
    persist (P<T>& obj_ptr);

    template <typename T>
    typename object_traits<T>::id_type
    persist (const typename object_traits<T>::pointer_type& obj_ptr);

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

    template <typename T>
    void
    update (T* obj_ptr);

    template <typename T, template <typename> class P>
    void
    update (const P<T>& obj_ptr);

    template <typename T, template <typename> class P>
    void
    update (P<T>& obj_ptr);

    template <typename T>
    void
    update (const typename object_traits<T>::pointer_type& obj_ptr);

    // Make the object transient. Throw object_not_persistent if not
    // found.
    //
    template <typename T>
    void
    erase (const typename object_traits<T>::id_type& id);

    template <typename T>
    void
    erase (T& object);

    template <typename T>
    void
    erase (T* obj_ptr);

    template <typename T, template <typename> class P>
    void
    erase (const P<T>& obj_ptr);

    template <typename T, template <typename> class P>
    void
    erase (P<T>& obj_ptr);

    template <typename T>
    void
    erase (const typename object_traits<T>::pointer_type& obj_ptr);

    // Erase multiple objects matching a query predicate.
    //
    template <typename T>
    unsigned long long
    erase_query ();

    template <typename T>
    unsigned long long
    erase_query (const char*);

    template <typename T>
    unsigned long long
    erase_query (const std::string&);

    template <typename T>
    unsigned long long
    erase_query (const odb::query<typename object_traits<T>::object_type>&);

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
    query (const odb::query<typename details::meta::remove_const<T>::result>&,
           bool cache = true);

    // Native database statement execution.
    //
  public:
    unsigned long long
    execute (const char* statement);

    unsigned long long
    execute (const std::string& statement);

    unsigned long long
    execute (const char* statement, std::size_t length);

    // Transactions.
    //
  public:
    virtual transaction_impl*
    begin () = 0;

    // Connections.
    //
  public:
    connection_ptr
    connection ();

  protected:
    database ();

  protected:
    typedef odb::connection connection_type;

    virtual connection_type*
    connection_ () = 0;

  protected:
    template <typename T>
    typename object_traits<T>::id_type
    persist_ (const typename object_traits<T>::pointer_type&);

    template <typename T>
    void
    update_ (const typename object_traits<T>::pointer_type&);

    template <typename T>
    void
    erase_ (const typename object_traits<T>::pointer_type&);

    template <typename T, class_kind kind>
    struct query_;

  private:
    database (const database&);
    database& operator= (const database&);
  };
}

#include <odb/database.ixx>
#include <odb/database.txx>

#include <odb/post.hxx>

#endif // ODB_DATABASE_HXX
