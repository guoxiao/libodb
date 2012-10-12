// file      : odb/connection.cxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/database.hxx>
#include <odb/connection.hxx>
#include <odb/exceptions.hxx> // prepared_*

using namespace std;

namespace odb
{
  connection::
  ~connection ()
  {
    for (prepared_map_type::iterator i (prepared_map_.begin ()),
           e (prepared_map_.end ()); i != e; ++i)
    {
      if (i->second.params != 0)
        i->second.params_deleter (i->second.params);
    }
  }

  void connection::
  cache_query_ (details::shared_ptr<prepared_query_impl> pq,
                const type_info& ti,
                void* params,
                const type_info* params_info,
                void (*params_deleter) (void*))
  {
    pair<prepared_map_type::iterator, bool> r (
      prepared_map_.insert (
        prepared_map_type::value_type (pq->name, prepared_entry_type ())));

    if (!r.second)
      throw prepared_already_cached (pq->name);

    prepared_entry_type& e (r.first->second);

    e.prep_query = pq;
    e.type_info = &ti;

    // Mark the statement as cached.
    //
    pq->stmt->cached (true);

    e.params = params;
    e.params_info = params_info;
    e.params_deleter = params_deleter;
  }

  details::shared_ptr<prepared_query_impl> connection::
  lookup_query_ (const char* name,
                 const type_info& ti,
                 void** params,
                 const type_info* params_info) const
  {
    prepared_map_type::const_iterator i (prepared_map_.find (name));

    if (i == prepared_map_.end ())
    {
      // See if there is a factory.
      //
      database_type::query_factory_type f (
        database_.lookup_query_factory (name));

      if (f)
      {
        f (name, const_cast<connection&> (*this));
        i = prepared_map_.find (name);
      }
    }

    if (i == prepared_map_.end ())
      return details::shared_ptr<prepared_query_impl> ();

    // Make sure the types match.
    //
    if (*i->second.type_info != ti)
      throw prepared_type_mismatch (name);

    if (params != 0)
    {
      if (*i->second.params_info != *params_info)
        throw prepared_type_mismatch (name);

      *params = i->second.params;
    }

    return i->second.prep_query;
  }
}
