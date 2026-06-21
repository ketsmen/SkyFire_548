/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_DATABASE_SETUP_H
#define SKYFIRE_DATABASE_SETUP_H

#include <functional>
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
        std::string Domain;
        std::string SqlPath;
        std::string BaseFileName;
        std::string UpdatesDirectory;
    };

    struct SetupState
    {
        bool DatabaseExists = false;
        bool UpdateTrackingExists = false;
        unsigned int SchemaTableCount = 0;
        std::set<std::string> AppliedUpdates;
    };

    struct SqlUpdateFile
    {
        std::string Name;
        std::string Path;
    };

    struct SetupPlan
    {
        bool ShouldCreateDatabase = false;
        bool ShouldInstallBase = false;
        std::vector<SqlUpdateFile> PendingUpdates;
        std::string Error;

        bool IsValid() const;
    };

    SetupOptions MakeAuthDatabaseSetupOptions(bool autoSetup, bool autoCreate, std::string sqlPath);
    std::vector<SqlUpdateFile> BuildSortedSqlUpdateList(std::vector<std::string> const& names, std::string const& directory);
    std::vector<SqlUpdateFile> DiscoverSqlUpdates(SetupOptions const& options);
    std::vector<std::string> SplitSqlStatements(std::string const& sql);
    bool ExecuteSqlScript(std::string const& sql, std::function<bool(std::string const&)> const& executor);
    std::string CalculateStableSqlHash(std::string const& sql);
    std::string EscapeSqlString(std::string const& value);
    SetupPlan BuildAuthDatabaseSetupPlan(SetupOptions const& options, SetupState const& state, bool baseSqlExists,
        std::vector<SqlUpdateFile> const& updates);
}
}

#endif
