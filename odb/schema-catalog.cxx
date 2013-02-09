// file      : odb/schema-catalog.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <map>
#include <vector>

#include <odb/database.hxx>
#include <odb/exceptions.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/schema-catalog-impl.hxx>

using namespace std;

namespace odb
{
  // It is important we use vector to store the list of create
  // functions since schema generators for some databases may
  // rely on the DDL statements executing in a specific order,
  // for example, for foreign key generation.
  //
  typedef bool (*create_function) (database&, unsigned short pass, bool drop);
  typedef pair<database_id, string> key;
  typedef vector<create_function> create_functions;
  struct schema_catalog_impl: map<key, create_functions> {};

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

    const create_functions& fs (i->second);

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

  // schema_catalog_entry
  //
  schema_catalog_entry::
  schema_catalog_entry (database_id id, const char* name, create_function cf)
  {
    schema_catalog_impl& c (*schema_catalog_init::catalog);
    c[key(id, name)].push_back (cf);
  }
}
