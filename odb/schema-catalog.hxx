// file      : odb/schema-catalog.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SCHEMA_CATALOG_HXX
#define ODB_SCHEMA_CATALOG_HXX

#include <odb/pre.hxx>

#include <odb/details/config.hxx> // ODB_CXX11

#include <string>
#include <cstddef> // std::size_t

#ifdef ODB_CXX11
#  include <functional> // std::function
#endif

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
  public:
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

    // Data migration.
    //
  public:
    // If version is 0, then use the current database version and also
    // check whether we are in migration. Returns the number of calls made.
    //
    static std::size_t
    migrate_data (database&,
                  schema_version = 0,
                  const std::string& name = "");

#ifdef ODB_CXX11
    typedef std::function<void (database&)> data_migration_function_type;
#else
    typedef void (*data_migration_function_type) (database&);
#endif

    // Data migration functions are called in the order of registration.
    //
    static void
    data_migration_function (schema_version v,
                             data_migration_function_type f,
                             const std::string& name = "")
    {
      data_migration_function (id_common, v, f, name);
    }

    // Database-specific data migration.
    //
    static void
    data_migration_function (database& db,
                             schema_version v,
                             data_migration_function_type f,
                             const std::string& name = "")
    {
      data_migration_function (db.id (), v, f, name);
    }

    static void
    data_migration_function (database_id,
                             schema_version,
                             data_migration_function_type,
                             const std::string& name = "");

    // Combined schema and data migration.
    //
  public:
    // Migrate both schema and data to the specified version. If version
    // is not specified, then migrate to the current model version.
    //
    static void
    migrate (database&, schema_version = 0, const std::string& name = "");

    // Schema version information.
    //
  public:
    // Return the current model version.
    //
    static schema_version
    current_version (const database& db, const std::string& name = "")
    {
      return current_version (db.id (), name);
    }

    static schema_version
    current_version (database_id, const std::string& name = "");

    // Return current model version + 1 (that is, one past current) if
    // the passed version is equal to or greater than current. If the
    // version is not specified, then use the current database version.
    //
    static schema_version
    next_version (const database& db,
                  schema_version v = 0,
                  const std::string& name = "")
    {
      return next_version (db.id (), v == 0 ? db.schema_version () : v, name);
    }

    static schema_version
    next_version (database_id,
                  schema_version,
                  const std::string& name = "");

    // Schema existence.
    //
  public:
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

  // Static data migration function registration.
  //
  struct LIBODB_EXPORT data_migration_entry
  {
    typedef schema_catalog::data_migration_function_type function_type;

    data_migration_entry (schema_version v,
                          function_type f,
                          const std::string& name = "")
    {
      schema_catalog::data_migration_function (v, f, name);
    }

    data_migration_entry (database_id id,
                          schema_version v,
                          function_type f,
                          const std::string& name = "")
    {
      schema_catalog::data_migration_function (id, v, f, name);
    }
  };

  namespace common
  {
    using odb::schema_catalog;
    using odb::data_migration_entry;
  }
}

#include <odb/post.hxx>

#endif // ODB_SCHEMA_CATALOG_HXX
