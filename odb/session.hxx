// file      : odb/session.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SESSION_HXX
#define ODB_SESSION_HXX

#include <odb/pre.hxx>

#include <map>
#include <typeinfo>

#include <odb/traits.hxx>
#include <odb/forward.hxx>

#include <odb/details/shared-ptr.hxx>
#include <odb/details/type-info.hxx>

#include <odb/details/export.hxx>

namespace odb
{
  class LIBODB_EXPORT session
  {
  public:
    typedef odb::database database_type;

    // If the make_current argument is true, then set the current thread's
    // session to this session. If another session is already in effect,
    // throw the already_in_session exception.
    //
    session (bool make_current = true);

    // Reset the current thread's session if it is this session.
    //
    ~session ();

    // Current session.
    //
  public:
    // Return true if there is a session in effect in the current
    // thread.
    //
    static bool
    has_current ();

    // Get current thread's session. Throw if no session is in effect.
    //
    static session&
    current ();

    // Set current thread's session.
    //
    static void
    current (session&);

    // Revert to the no session in effect state for the current thread.
    //
    static void
    reset_current ();

    // Copying or assignment of sessions is not supported.
    //
  private:
    session (const session&);
    session& operator= (const session&);

  protected:
    struct LIBODB_EXPORT object_map_base: details::shared_base
    {
      virtual
      ~object_map_base ();
    };

    template <typename T>
    struct object_map:
      object_map_base,
      std::map<typename object_traits<T>::id_type,
               typename object_traits<T>::pointer_type>
    {
    };

    // Object cache.
    //
  public:
    template <typename T>
    struct object_position
    {
      typedef T object_type;
      typedef object_map<object_type> map;
      typedef typename map::iterator iterator;

      object_position (): map_ (0) {}
      object_position (map& m, const iterator& p): map_ (&m), pos_ (p) {}

      map* map_;
      iterator pos_;
    };

    template <typename T>
    object_position<T>
    insert (database_type&,
            const typename object_traits<T>::id_type&,
            const typename object_traits<T>::pointer_type&);

    template <typename T>
    typename object_traits<T>::pointer_type
    find (database_type&, const typename object_traits<T>::id_type&) const;

    template <typename T>
    void
    erase (database_type&, const typename object_traits<T>::id_type&);

    template <typename T>
    void
    erase (const object_position<T>&);

  protected:
    typedef std::map<const std::type_info*,
                     details::shared_ptr<object_map_base>,
                     details::type_info_comparator> type_map;

    typedef std::map<database_type*, type_map> database_map;

    database_map db_map_;
  };
}

#include <odb/session.ixx>
#include <odb/session.txx>

#include <odb/post.hxx>

#endif // ODB_SESSION_HXX
