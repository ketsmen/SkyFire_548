/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_DATABASE_SETUP_H
#define SKYFIRE_DATABASE_SETUP_H

#include <cstddef>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace Skyfire
{
namespace Database
{
    struct SetupOptions
    {
        bool AutoSetup = false;
        bool AutoCreate = false;
        bool AutoBaseline = false;
        std::string Domain;
        std::string SqlPath;
        std::string BaseFileName;
        std::string ExternalBaseFile;
        std::vector<std::string> RequiredBaseFileNames;
        std::string UpdatesDirectory;
    };

    struct SetupState
    {
        bool DatabaseExists = false;
        bool UpdateTrackingExists = false;
        unsigned int SchemaTableCount = 0;
        std::set<std::string> AppliedUpdates;
        std::map<std::string, std::string> AppliedUpdateHashes;
    };

    struct SqlUpdateFile
    {
        std::string Name;
        std::string Path;
        std::string Hash;
    };

    struct SetupPlan
    {
        bool ShouldCreateDatabase = false;
        bool ShouldInstallBase = false;
        bool ShouldBaselineUpdates = false;
        std::vector<SqlUpdateFile> PendingUpdates;
        std::vector<SqlUpdateFile> BaselineUpdates;
        std::string Error;

        bool IsValid() const;
    };

    SetupOptions MakeAuthDatabaseSetupOptions(bool autoSetup, bool autoCreate, std::string sqlPath);
    SetupOptions MakeAuthDatabaseSetupOptions(bool autoSetup, bool autoCreate, bool autoBaseline, std::string sqlPath);
    SetupOptions MakeCharacterDatabaseSetupOptions(bool autoSetup, bool autoCreate, std::string sqlPath);
    SetupOptions MakeCharacterDatabaseSetupOptions(bool autoSetup, bool autoCreate, bool autoBaseline, std::string sqlPath);
    SetupOptions MakeWorldDatabaseSetupOptions(bool autoSetup, bool autoCreate, std::string sqlPath,
        std::string externalBaseFile);
    SetupOptions MakeWorldDatabaseSetupOptions(bool autoSetup, bool autoCreate, bool autoBaseline, std::string sqlPath,
        std::string externalBaseFile);
    std::vector<SqlUpdateFile> BuildSortedSqlUpdateList(std::vector<std::string> const& names, std::string const& directory);
    std::vector<SqlUpdateFile> DiscoverSqlUpdates(SetupOptions const& options);
    std::vector<std::string> SplitSqlStatements(std::string const& sql);
    bool ExecuteSqlScript(std::string const& sql, std::function<bool(std::string const&)> const& executor);
    std::string CalculateStableSqlHash(std::string const& sql);
    std::string EscapeSqlString(std::string const& value);
    std::string EscapeSqlIdentifier(std::string const& identifier);
    std::string BuildCreateDatabaseSql(std::string const& databaseName);
    std::string BuildUpdateTrackingTableSql();
    std::string BuildUpdateTrackingInsertSql(std::string const& domain, std::string const& filename,
        std::string const& hash);
    std::string BuildDbUpdateAuditTableSql();
    std::string BuildDbUpdateAuditInsertSql(std::string const& filename);
    std::string BuildSetupPlanSummary(std::string const& databaseName, SetupPlan const& plan,
        std::size_t discoveredUpdateCount, bool appliesRequiredSql);
    SetupPlan BuildDatabaseSetupPlan(SetupOptions const& options, SetupState const& state, bool baseSqlExists,
        std::vector<SqlUpdateFile> const& updates);
    SetupPlan BuildAuthDatabaseSetupPlan(SetupOptions const& options, SetupState const& state, bool baseSqlExists,
        std::vector<SqlUpdateFile> const& updates);
    SetupPlan BuildCharacterDatabaseSetupPlan(SetupOptions const& options, SetupState const& state, bool baseSqlExists,
        std::vector<SqlUpdateFile> const& updates);
    SetupPlan BuildWorldDatabaseSetupPlan(SetupOptions const& options, SetupState const& state, bool externalBaseSqlExists,
        bool requiredBaseSqlExists, std::vector<SqlUpdateFile> const& updates);
}
}

#endif
