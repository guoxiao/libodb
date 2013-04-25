// file      : odb/schema-catalog.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SCHEMA_CATALOG_HXX
#define ODB_SCHEMA_CATALOG_HXX

#include <odb/pre.hxx>

#include <string>

#include <odb/forward.hxx> // schema_version, odb::core

#include <odb/details/export.hxx>

namespace odb
{
  class LIBODB_EXPORT schema_catalog
  {
  public:
    // Schema creation.
    //
    static void
    create_schema (database&, const std::string& name = "", bool drop = true);

    static void
    drop_schema (database&, const std::string& name = "");

    // Schema migration.
    //
    static void
    migrate_schema_pre (database& db,
                        schema_version v,
                        const std::string& name = "")
    {
      migrate_schema_impl (db, v, name, migrate_pre);
    }

    static void
    migrate_schema_post (database& db,
                         schema_version v,
                         const std::string& name = "")
    {
      migrate_schema_impl (db, v, name, migrate_post);
    }

    static void
    migrate_schema (database& db,
                    schema_version v,
                    const std::string& name = "")
    {
      migrate_schema_impl (db, v, name, migrate_both);
    }

    // Return 0 if current is greater or equal to the latest version.
    //
    schema_version
    next_version (const database& db,
                  schema_version current /*= 0*/,
                  const std::string& name = "")
    {
      return next_version (db.id (), current, name);
    }

    schema_version
    next_version (database_id,
                  schema_version current /*= 0*/,
                  const std::string& name = "");

    schema_version
    latest_version (const database& db, const std::string& name = "")
    {
      return latest_version (db.id (), name);
    }

    schema_version
    latest_version (database_id, const std::string& name = "");

    // Test for presence of a schema with a specific name.
    //
    static bool
    exists (const database& db, const std::string& name = "")
    {
      return exists (db.id (), name);
    }

    static bool
    exists (database_id, const std::string& name = "");

  private:
    enum migrate_mode
    {
      migrate_pre,
      migrate_post,
      migrate_both
    };

    static void
    migrate_schema_impl (database&,
                         schema_version,
                         const std::string& name,
                         migrate_mode);
  };

  namespace common
  {
    using odb::schema_catalog;
  }
}

#include <odb/post.hxx>

#endif // ODB_SCHEMA_CATALOG_HXX
