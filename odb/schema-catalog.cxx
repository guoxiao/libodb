// file      : odb/schema-catalog.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <map>
#include <vector>

#include <odb/exceptions.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/schema-catalog-impl.hxx>

using namespace std;

namespace odb
{
  typedef void (*create_function) (database&);
  typedef vector<create_function> create_functions;
  struct schema_catalog_impl: map<string, create_functions> {};

  schema_catalog_impl* schema_catalog_init::catalog = 0;
  size_t schema_catalog_init::count = 0;

  void schema_catalog::
  create_schema (database& db, const string& name)
  {
    const schema_catalog_impl& c (*schema_catalog_init::catalog);

    schema_catalog_impl::const_iterator i (c.find (name));

    if (i == c.end ())
      throw unknown_schema (name);

    const create_functions& fs (i->second);

    for (create_functions::const_iterator j (fs.begin ()), e (fs.end ());
         j != e; ++j)
      (*j) (db);
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
  schema_catalog_entry (const char* name, create_function entry)
  {
    schema_catalog_impl& c (*schema_catalog_init::catalog);
    c[name].push_back (entry);
  }
}
