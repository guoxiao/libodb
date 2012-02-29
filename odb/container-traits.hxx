// file      : odb/container-traits.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_CONTAINER_TRAITS_HXX
#define ODB_CONTAINER_TRAITS_HXX

#include <odb/pre.hxx>

#include <odb/forward.hxx>
#include <odb/details/config.hxx> // ODB_CXX11

namespace odb
{
  // Keep this enum synchronized with the one in odb/odb/context.hxx.
  //
  enum container_kind
  {
    ck_ordered,
    ck_set,
    ck_multiset,
    ck_map,
    ck_multimap
  };

  //
  // Container API provided by the generated code.
  //

  // Ordered containers.
  //
  template <typename I, typename V>
  struct ordered_functions
  {
    typedef I index_type;
    typedef V value_type;

    // Return true if the order is preserved in the database. If the
    // order is not preserved, then the index argument in the functions
    // below is not used.
    //
    bool
    ordered () const
    {
      return ordered_;
    }

    void
    insert_one (I index, const V& value) const
    {
      insert_one_ (index, value, data_);
    }

    bool
    load_all (I& next_index, V& next_value) const
    {
      return load_all_ (next_index, next_value, data_);
    }

    void
    delete_all () const
    {
      delete_all_ (data_);
    }

    // Implementation details.
    //
  public:
    typedef void (*insert_one_type) (I, const V&, void*);
    typedef bool (*load_all_type) (I&, V&, void*);
    typedef void (*delete_all_type) (void*);

    ordered_functions (void* data,
                       insert_one_type io,
                       load_all_type la,
                       delete_all_type da)
        : data_ (data), insert_one_ (io), load_all_ (la), delete_all_ (da)
    {
    }

    void
    ordered (bool v)
    {
      ordered_ = v;
    }

  private:
    void* data_;
    bool ordered_;
    insert_one_type insert_one_;
    load_all_type load_all_;
    delete_all_type delete_all_;
  };

  // Set/multiset containers.
  //
  template <typename V>
  struct set_functions
  {
    typedef V value_type;

    void
    insert_one (const V& value) const
    {
      insert_one_ (value, data_);
    }

    bool
    load_all (V& next_value) const
    {
      return load_all_ (next_value, data_);
    }

    void
    delete_all () const
    {
      delete_all_ (data_);
    }

    // Implementation details.
    //
  public:
    typedef void (*insert_one_type) (const V&, void*);
    typedef bool (*load_all_type) (V&, void*);
    typedef void (*delete_all_type) (void*);

    set_functions (void* data,
                   insert_one_type io,
                   load_all_type la,
                   delete_all_type da)
        : data_ (data), insert_one_ (io), load_all_ (la), delete_all_ (da)
    {
    }

  private:
    void* data_;
    insert_one_type insert_one_;
    load_all_type load_all_;
    delete_all_type delete_all_;
  };

  // Map/multimap containers.
  //
  template <typename K, typename V>
  struct map_functions
  {
    typedef K key_type;
    typedef V value_type;

    void
    insert_one (const K& key, const V& value) const
    {
      insert_one_ (key, value, data_);
    }

    bool
    load_all (K& next_key, V& next_value) const
    {
      return load_all_ (next_key, next_value, data_);
    }

    void
    delete_all () const
    {
      delete_all_ (data_);
    }

    // Implementation details.
    //
  public:
    typedef void (*insert_one_type) (const K&, const V&, void*);
    typedef bool (*load_all_type) (K&, V&, void*);
    typedef void (*delete_all_type) (void*);

    map_functions (void* data,
                   insert_one_type io,
                   load_all_type la,
                   delete_all_type da)
        : data_ (data), insert_one_ (io), load_all_ (la), delete_all_ (da)
    {
    }

  private:
    void* data_;
    insert_one_type insert_one_;
    load_all_type load_all_;
    delete_all_type delete_all_;
  };
}

#include <odb/post.hxx>

#include <odb/std-map-traits.hxx>
#include <odb/std-set-traits.hxx>
#include <odb/std-list-traits.hxx>
#include <odb/std-vector-traits.hxx>

#ifdef ODB_CXX11
#  include <odb/std-array-traits.hxx>
#  include <odb/std-forward-list-traits.hxx>
#  include <odb/std-unordered-map-traits.hxx>
#  include <odb/std-unordered-set-traits.hxx>
#endif

#endif // ODB_CONTAINER_TRAITS_HXX
