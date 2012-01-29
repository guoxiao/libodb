// file      : odb/schema-catalog.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SCHEMA_CATALOG_HXX
#define ODB_SCHEMA_CATALOG_HXX

#include <odb/pre.hxx>

#include <string>

#include <odb/forward.hxx>

#include <odb/details/export.hxx>

namespace odb
{
  class LIBODB_EXPORT schema_catalog
  {
  public:
    static void
    create_schema (database&, const std::string& name = "");
  };

  namespace core
  {
    using odb::schema_catalog;
  }
}

#include <odb/post.hxx>

#endif // ODB_SCHEMA_CATALOG_HXX
