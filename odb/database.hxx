// file      : odb/database.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
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

    template <typename T, typename A1, template <typename, typename> class P>
    typename object_traits<T>::id_type
    persist (const P<T, A1>& obj_ptr);

    template <typename T, template <typename> class P>
    typename object_traits<T>::id_type
    persist (P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    typename object_traits<T>::id_type
    persist (P<T, A1>& obj_ptr);

    template <typename T>
    typename object_traits<T>::id_type
    persist (const typename object_traits<T>::pointer_type& obj_ptr);

    // Load an object. Throw object_not_persistent if not found.
    //
    template <typename T>
    typename object_traits<T>::pointer_type
    load (const typename object_traits<T>::id_type& id);

    template <typename T>
    void
    load (const typename object_traits<T>::id_type& id, T& object);

    // Reload an object.
    //
    template <typename T>
    void
    reload (T& object);

    template <typename T>
    void
    reload (T* obj_ptr);

    template <typename T, template <typename> class P>
    void
    reload (const P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    void
    reload (const P<T, A1>& obj_ptr);

    template <typename T, template <typename> class P>
    void
    reload (P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    void
    reload (P<T, A1>& obj_ptr);

    template <typename T>
    void
    reload (const typename object_traits<T>::pointer_type& obj_ptr);

    // Loan an object if found. Return NULL/false if not found.
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

    template <typename T, typename A1, template <typename, typename> class P>
    void
    update (const P<T, A1>& obj_ptr);

    template <typename T, template <typename> class P>
    void
    update (P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    void
    update (P<T, A1>& obj_ptr);

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

    template <typename T, typename A1, template <typename, typename> class P>
    void
    erase (const P<T, A1>& obj_ptr);

    template <typename T, template <typename> class P>
    void
    erase (P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    void
    erase (P<T, A1>& obj_ptr);

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
    erase_query (const odb::query<T>&);

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

    // SQL statement tracing.
    //
  public:
    typedef odb::tracer tracer_type;

    void
    tracer (tracer_type&);

    void
    tracer (tracer_type*);

    tracer_type*
    tracer () const;

    // Database id.
    //
  public:
    database_id
    id () const;

  protected:
    database (database_id);

  private:
    database (const database&);
    database& operator= (const database&);

  protected:
    typedef odb::connection connection_type;

    virtual connection_type*
    connection_ () = 0;

  protected:
    template <typename T, database_id DB>
    typename object_traits<T>::id_type
    persist_ (T&);

    template <typename T, database_id DB>
    typename object_traits<T>::id_type
    persist_ (const typename object_traits<T>::pointer_type&);

    template <typename T, database_id DB>
    typename object_traits<T>::pointer_type
    load_ (const typename object_traits<T>::id_type&);

    template <typename T, database_id DB>
    void
    load_ (const typename object_traits<T>::id_type&, T&);

    template <typename T, database_id DB>
    void
    reload_ (T&);

    template <typename T, database_id DB>
    typename object_traits<T>::pointer_type
    find_ (const typename object_traits<T>::id_type&);

    template <typename T, database_id DB>
    bool
    find_ (const typename object_traits<T>::id_type&, T&);

    template <typename T, database_id DB>
    void
    update_ (T&);

    template <typename T, database_id DB>
    void
    update_ (const typename object_traits<T>::pointer_type&);

    template <typename T, database_id DB>
    void
    erase_ (const typename object_traits<T>::id_type&);

    template <typename T, database_id DB>
    void
    erase_ (T&);

    template <typename T, database_id DB>
    void
    erase_ (const typename object_traits<T>::pointer_type&);

    template <typename T,
              database_id DB,
              class_kind kind = class_traits<T>::kind>
    struct query_;

  protected:
    database_id id_;
    tracer_type* tracer_;
  };
}

#include <odb/database.ixx>
#include <odb/database.txx>

#include <odb/post.hxx>

#endif // ODB_DATABASE_HXX
