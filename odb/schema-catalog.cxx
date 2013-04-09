// file      : odb/schema-catalog.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <map>
#include <vector>
#include <cassert>

#include <odb/database.hxx>
#include <odb/exceptions.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/schema-catalog-impl.hxx>

using namespace std;

namespace odb
{
  typedef bool (*create_function) (database&, unsigned short pass, bool drop);
  typedef bool (*migrate_function) (database&, unsigned short pass, bool pre);

  typedef pair<database_id, string> key;
  typedef vector<create_function> create_functions;
  typedef vector<migrate_function> migrate_functions;
  typedef map<schema_version, migrate_functions> version_map;

  struct schema_functions
  {
    create_functions create;
    version_map migrate;
  };

  struct schema_catalog_impl: map<key, schema_functions> {};

  schema_catalog_impl* schema_catalog_init::catalog = 0;
  size_t schema_catalog_init::count = 0;

  bool schema_catalog::
  exists (database_id id, const std::string& name)
  {
    const schema_catalog_impl& c (*schema_catalog_init::catalog);
    return c.find (key (id, name)) != c.end ();
  }

  void schema_catalog::
  create_schema (database& db, const string& name)
  {
    const schema_catalog_impl& c (*schema_catalog_init::catalog);
    schema_catalog_impl::const_iterator i (c.find (key (db.id (), name)));

    if (i == c.end ())
      throw unknown_schema (name);

    const create_functions& fs (i->second.create);

    // Run the passes until we ran them all or all the functions
    // return false, which means no more passes necessary. Do that
    // first for drop passes, then for create.
    //

    for (unsigned short pass (1); pass < 3; ++pass)
    {
      bool done (true);

      for (create_functions::const_iterator j (fs.begin ()), e (fs.end ());
           j != e; ++j)
      {
        if ((*j) (db, pass, true))
          done = false;
      }

      if (done)
        break;
    }

    for (unsigned short pass (1); pass < 3; ++pass)
    {
      bool done (true);

      for (create_functions::const_iterator j (fs.begin ()), e (fs.end ());
           j != e; ++j)
      {
        if ((*j) (db, pass, false))
          done = false;
      }

      if (done)
        break;
    }
  }

  void schema_catalog::
  migrate_schema_impl (database& db,
                       schema_version v,
                       const string& name,
                       migrate_mode m)
  {
    const schema_catalog_impl& c (*schema_catalog_init::catalog);
    schema_catalog_impl::const_iterator i (c.find (key (db.id (), name)));

    if (i == c.end ())
      throw unknown_schema (name);

    const version_map& vm (i->second.migrate);
    version_map::const_iterator j (vm.find (v));

    if (j == vm.end ())
      throw unknown_schema_version (v);

    const migrate_functions& fs (j->second);

    // Run the passes until we ran them all or all the functions
    // return false, which means no more passes necessary.
    //
    for (bool pre (m != migrate_post);; pre = false)
    {
      for (unsigned short pass (1); pass < 3; ++pass)
      {
        bool done (true);

        for (migrate_functions::const_iterator i (fs.begin ()), e (fs.end ());
             i != e; ++i)
        {
          if ((*i) (db, pass, pre))
            done = false;
        }

        if (done)
          break;
      }

      if (!pre || m != migrate_both)
        break;
    }
  }

  schema_version schema_catalog::
  next_version (database_id id, schema_version current, const string& name)
  {
    const schema_catalog_impl& c (*schema_catalog_init::catalog);
    schema_catalog_impl::const_iterator i (c.find (key (id, name)));

    if (i == c.end ())
      throw unknown_schema (name);

    const version_map& vm (i->second.migrate);
    version_map::const_iterator j (vm.upper_bound (current));
    return j != vm.end () ? j->first : 0;
  }

  schema_version schema_catalog::
  latest_version (database_id id, const string& name)
  {
    const schema_catalog_impl& c (*schema_catalog_init::catalog);
    schema_catalog_impl::const_iterator i (c.find (key (id, name)));

    if (i == c.end ())
      throw unknown_schema (name);

    const version_map& vm (i->second.migrate);
    assert (!vm.empty ());
    return vm.rbegin ()->first;
  }

  // schema_catalog_init
  //
  schema_catalog_init::
  schema_catalog_init ()
  {
    if (count == 0)
      catalog = new schema_catalog_impl;

    ++count;
  }

  schema_catalog_init::
  ~schema_catalog_init ()
  {
    if (--count == 0)
      delete catalog;
  }

  // schema_catalog_create_entry
  //
  schema_catalog_create_entry::
  schema_catalog_create_entry (database_id id,
                               const char* name,
                               create_function cf)
  {
    schema_catalog_impl& c (*schema_catalog_init::catalog);
    c[key(id, name)].create.push_back (cf);
  }

  // schema_catalog_migrate_entry
  //
  schema_catalog_migrate_entry::
  schema_catalog_migrate_entry (database_id id,
                                const char* name,
                                schema_version v,
                                migrate_function mf)
  {
    schema_catalog_impl& c (*schema_catalog_init::catalog);
    c[key(id, name)].migrate[v].push_back (mf);
  }
}
