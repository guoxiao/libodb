// file      : odb/function-table.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_FUNCTION_TABLE_HXX
#define ODB_FUNCTION_TABLE_HXX

#include <odb/pre.hxx>

#include <odb/forward.hxx>

namespace odb
{
  template <typename T, database_id DB>
  struct function_table_entry
  {
    typedef access::object_traits_impl<T, id_default> default_traits;

    function_table_entry (
      const typename default_traits::function_table_type* t)
    {
      default_traits::function_table[DB] = t;
    }

    ~function_table_entry ()
    {
      default_traits::function_table[DB] = 0;
    }
  };
}

#include <odb/post.hxx>

#endif // ODB_FUNCTION_TABLE_HXX
