// file      : odb/session.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SESSION_HXX
#define ODB_SESSION_HXX

#include <map>
#include <typeinfo>

#include <odb/forward.hxx>
#include <odb/traits.hxx>

namespace odb
{
  class session
  {
  public:
    session ();
    ~session ()
    {
      reset_current ();
    }

  private:
    session (const session&);
    session& operator= (const session&);

  public:
    template <typename T, template <typename> class P>
    typename object_traits<T>::id_type
    persist (database&, P<T> obj);

    template <typename T>
    typename object_traits<T>::shared_ptr
    find (database&, typename object_traits<T>::id_type const&);

    template <typename T, template <typename> class P>
    void
    erase (database&, P<T> obj);

    template <typename T, template <typename> class P>
    void
    modified (P<T> obj);

    void
    flush ();

  public:
    // Return true if there is a session in effect.
    //
    static bool
    has_current ();

    // Get current thread's session. Throw if no session in effect.
    //
    static session&
    current ();

    // Set current thread's session.
    //
    static void
    current (session&);

    // Revert to the no session in effect state.
    //
    static void
    reset_current ();

  private:
    struct object_proxy;
    typedef std::map< void*, shared_ptr<object_proxy> > object_map;

    struct type_map
    {
      virtual
      ~type_map () = 0;
    };

    template <typename T>
    struct type_map_impl:
      type_map,
      std::map<typename object_traits<T>::id_type, object_map::iterator>
    {
    };

    struct type_info_comparator
    {
      bool
      operator() (const std::type_info* x, const std::type_info* y) const
      {
        // XL C++ on AIX has buggy type_info::before() in that
        // it returns true for two different type_info objects
        // that happened to be for the same type.
        //
#if defined(__xlC__) && defined(_AIX)
        return *x != *y && x->before (*y);
#else
        return x->before (*y);
#endif
      }
    };

    typedef
    std::map<const std::type_info*, shared_ptr<type_map>, type_info_comparator>
    id_map;

    //
    //
    struct object_proxy
    {
      virtual
      ~object_proxy () = 0;

      object_proxy (database& db, id_source is)
          : id_source_ (is), db_ (db)
      {
      }

      enum state
      {
        transient,  /* Persisted out of transaction. */
        clean,
        dirty,      /* To be updated on flush. */
        erased      /* To be deleted on flush. */
      };

    public:
      virtual void
      persist () = 0;

      virtual void
      update () = 0;

      virtual void
      erase () = 0;

      virtual void
      register_id (id_map&, object_map::iterator) = 0;

      virtual void
      unregister_id (id_map&) = 0;

    public:
      state state_;
      id_source id_source_;
      database& db_;
    };

    template <typename T>
    struct object_proxy_impl: object_proxy
    {
      typedef object_traits<T> traits;
      typedef typename traits::shared_ptr obj_ptr;
      typedef typename traits::shared_ops ptr_ops;

      object_proxy_impl (database& db, obj_ptr obj)
          : object_proxy (db, traits::id_source), obj_ (obj)
      {
      }

    public:
      virtual void
      persist ();

      virtual void
      update ();

      virtual void
      erase ();

      virtual void
      register_id (id_map&, object_map::iterator);

      virtual void
      unregister_id (id_map&);

    public:
      obj_ptr obj_;
    };

    object_map object_map_;
    id_map id_map_;
  };
}

#include <odb/session.txx>

#endif // ODB_SESSION_HXX
