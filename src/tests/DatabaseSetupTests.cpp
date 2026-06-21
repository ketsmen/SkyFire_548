/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "DatabaseSetup.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool ExpectEqual(std::string const& actual, std::string const& expected, char const* message)
    {
        if (actual == expected)
            return true;

        std::cerr << message << '\n'
                  << "  actual:   " << actual << '\n'
                  << "  expected: " << expected << '\n';
        return false;
    }

    bool TestAuthDefaultsAreConservative()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(false, false, "");

        bool passed = true;
        passed &= Expect(options.Domain == "auth", "Auth setup domain should be auth");
        passed &= Expect(!options.AutoSetup, "Auth auto setup should default off");
        passed &= Expect(!options.AutoCreate, "Auth auto create should default off");
        passed &= Expect(!options.AutoBaseline, "Auth auto baseline should default off");
        passed &= Expect(options.SqlPath.empty(), "Auth setup SQL path should default empty");
        passed &= Expect(options.BaseFileName == "auth_database.sql", "Auth setup should use the auth base SQL file");
        passed &= Expect(options.UpdatesDirectory == "updates/auth", "Auth setup should use the auth updates directory");

        return passed;
    }

    bool TestExistingAuthDatabaseCanBaselineUpdates()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, true, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = false;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "hash-a" },
            { "2026-02-03_auth_00.sql", "sql/updates/auth/2026-02-03_auth_00.sql", "hash-b" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Existing auth schema should allow explicit baseline mode");
        passed &= Expect(!plan.ShouldInstallBase, "Baseline mode should not reinstall base SQL");
        passed &= Expect(plan.ShouldBaselineUpdates, "Baseline mode should record update metadata");
        passed &= Expect(plan.PendingUpdates.empty(), "Baseline mode should not execute historical updates");
        passed &= Expect(plan.BaselineUpdates.size() == 2, "Baseline mode should record all discovered updates");

        return passed;
    }

    bool TestExistingAuthDatabaseBaselineRequiresHashes()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, true, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = false;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(!plan.IsValid(), "Baseline mode should reject updates without content hashes");
        passed &= Expect(!plan.Error.empty(), "Baseline mode should explain missing content hashes");
        passed &= Expect(!plan.ShouldBaselineUpdates, "Baseline mode should not continue without hashes");

        return passed;
    }

    bool TestSqlUpdatesAreFilteredAndSorted()
    {
        std::vector<std::string> names =
        {
            "updates go here.txt",
            "2026-01-23_auth_00.sql",
            "2025-12-01_auth_00.sql",
            "2026-02-03_auth_00.SQL"
        };

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
            Skyfire::Database::BuildSortedSqlUpdateList(names, "sql/updates/auth");

        bool passed = true;
        passed &= Expect(updates.size() == 3, "Only SQL update files should be kept");
        passed &= Expect(updates[0].Name == "2025-12-01_auth_00.sql", "Auth updates should be sorted by filename");
        passed &= Expect(updates[1].Name == "2026-01-23_auth_00.sql", "Auth updates should preserve middle sorted filename");
        passed &= Expect(updates[2].Name == "2026-02-03_auth_00.SQL", "Auth update filtering should be case insensitive");
        passed &= Expect(updates[0].Path == "sql/updates/auth/2025-12-01_auth_00.sql", "Auth update path should include the directory");

        return passed;
    }

    bool TestEmptyAuthDatabaseInstallsBaseAndPendingUpdates()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 0;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Auth setup plan should be valid when base SQL exists");
        passed &= Expect(!plan.ShouldCreateDatabase, "Existing auth database should not be created");
        passed &= Expect(plan.ShouldInstallBase, "Empty auth database should install base SQL");
        passed &= Expect(plan.PendingUpdates.size() == 1, "Empty auth database should apply pending auth updates");

        return passed;
    }

    bool TestExistingAuthDatabaseSkipsAppliedUpdates()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = true;
        state.AppliedUpdates.insert("2026-01-23_auth_00.sql");
        state.AppliedUpdateHashes["2026-01-23_auth_00.sql"] = "hash-a";

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "hash-a" },
            { "2026-02-03_auth_00.sql", "sql/updates/auth/2026-02-03_auth_00.sql", "hash-b" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Auth setup plan should be valid for existing auth schema");
        passed &= Expect(!plan.ShouldInstallBase, "Existing auth schema should not reinstall base SQL");
        passed &= Expect(plan.PendingUpdates.size() == 1, "Existing auth schema should skip applied updates");
        passed &= Expect(plan.PendingUpdates[0].Name == "2026-02-03_auth_00.sql", "Existing auth schema should keep unapplied updates");

        return passed;
    }

    bool TestExistingAuthDatabaseRejectsChangedAppliedUpdate()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = true;
        state.AppliedUpdates.insert("2026-01-23_auth_00.sql");
        state.AppliedUpdateHashes["2026-01-23_auth_00.sql"] = "old-hash";

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "new-hash" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(!plan.IsValid(), "Changed applied auth update should fail version control validation");
        passed &= Expect(!plan.Error.empty(), "Changed applied auth update should explain the hash mismatch");
        passed &= Expect(plan.PendingUpdates.empty(), "Changed applied auth update should not be queued");

        return passed;
    }

    bool TestExistingAuthDatabaseWithoutTrackingFailsSafe()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = false;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(!plan.IsValid(), "Existing auth schema without tracking should fail safe");
        passed &= Expect(!plan.Error.empty(), "Missing tracking table plan should explain why setup stopped");
        passed &= Expect(plan.PendingUpdates.empty(), "Missing tracking table plan should not queue updates");

        return passed;
    }

    bool TestMissingDatabaseRequiresAutoCreate()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = false;

        Skyfire::Database::SetupPlan blockedPlan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, {});

        options.AutoCreate = true;
        Skyfire::Database::SetupPlan createPlan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, {});

        bool passed = true;
        passed &= Expect(!blockedPlan.IsValid(), "Missing auth database should fail when auto create is disabled");
        passed &= Expect(!blockedPlan.Error.empty(), "Blocked auth setup plan should explain the missing database");
        passed &= Expect(createPlan.IsValid(), "Missing auth database should be valid when auto create is enabled");
        passed &= Expect(createPlan.ShouldCreateDatabase, "Missing auth database should be created when auto create is enabled");
        passed &= Expect(createPlan.ShouldInstallBase, "New auth database should install base SQL");

        return passed;
    }

    bool TestSqlUpdatesAreDiscoveredFromConfiguredRoot()
    {
        std::filesystem::path root = std::filesystem::temp_directory_path() / "skyfire_database_setup_tests";
        std::filesystem::path updatesDir = root / "updates" / "auth";

        std::filesystem::remove_all(root);
        std::filesystem::create_directories(updatesDir);

        std::ofstream(updatesDir / "2026-03-01_auth_00.sql") << "SELECT 1;";
        std::ofstream(updatesDir / "readme.txt") << "notes";
        std::ofstream(updatesDir / "2026-01-23_auth_00.sql") << "SELECT 2;";

        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, root.string());
        std::vector<Skyfire::Database::SqlUpdateFile> updates =
            Skyfire::Database::DiscoverSqlUpdates(options);

        bool passed = true;
        passed &= Expect(updates.size() == 2, "Auth discovery should keep SQL files only");
        passed &= Expect(updates[0].Name == "2026-01-23_auth_00.sql", "Auth discovery should sort update filenames");
        passed &= Expect(updates[1].Name == "2026-03-01_auth_00.sql", "Auth discovery should preserve later sorted update");
        passed &= ExpectEqual(updates[0].Path, (updatesDir / "2026-01-23_auth_00.sql").string(),
            "Auth discovery should return full configured update paths");
        passed &= Expect(updates[0].Hash == Skyfire::Database::CalculateStableSqlHash("SELECT 2;"),
            "Auth discovery should hash update file contents for version control");

        std::filesystem::remove_all(root);

        return passed;
    }

    bool TestSqlScriptSplitsStatementsSafely()
    {
        std::string sql =
            "-- setup auth database\n"
            "CREATE TABLE `account` (`name` varchar(32));\n"
            "INSERT INTO `account` (`name`) VALUES ('semi;colon'), (\"double;semi\");\n"
            "/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;\n";

        std::vector<std::string> statements = Skyfire::Database::SplitSqlStatements(sql);

        bool passed = true;
        passed &= Expect(statements.size() == 3, "SQL splitter should return three executable statements");
        passed &= Expect(statements[0].find("CREATE TABLE") != std::string::npos,
            "SQL splitter should keep the first CREATE statement");
        passed &= Expect(statements[1].find("'semi;colon'") != std::string::npos,
            "SQL splitter should not split semicolons inside single quotes");
        passed &= Expect(statements[1].find("\"double;semi\"") != std::string::npos,
            "SQL splitter should not split semicolons inside double quotes");
        passed &= Expect(statements[2].find("/*!40101 SET SQL_MODE=@OLD_SQL_MODE */") != std::string::npos,
            "SQL splitter should keep MySQL conditional comments as statements");

        return passed;
    }

    bool TestSqlScriptExecutorStopsOnFailure()
    {
        std::vector<std::string> executed;
        bool result = Skyfire::Database::ExecuteSqlScript("SELECT 1; SELECT 2; SELECT 3;",
            [&executed](std::string const& statement)
            {
                executed.push_back(statement);
                return statement != "SELECT 2";
            });

        bool passed = true;
        passed &= Expect(!result, "SQL script execution should fail when a statement fails");
        passed &= Expect(executed.size() == 2, "SQL script execution should stop after the failed statement");
        passed &= Expect(executed[0] == "SELECT 1", "SQL script execution should run statements in order");
        passed &= Expect(executed[1] == "SELECT 2", "SQL script execution should include the failed statement");

        return passed;
    }

    bool TestStableSqlHash()
    {
        bool passed = true;
        passed &= Expect(Skyfire::Database::CalculateStableSqlHash("") == "cbf29ce484222325",
            "Stable SQL hash should use FNV-1a offset for empty content");
        passed &= Expect(Skyfire::Database::CalculateStableSqlHash("hello") == "a430d84680aabd0b",
            "Stable SQL hash should use deterministic FNV-1a");

        return passed;
    }

    bool TestSqlStringEscaping()
    {
        bool passed = true;
        passed &= Expect(Skyfire::Database::EscapeSqlString("plain") == "plain",
            "SQL string escaping should leave plain text unchanged");
        passed &= Expect(Skyfire::Database::EscapeSqlString("can't\\stop") == "can\\'t\\\\stop",
            "SQL string escaping should escape quotes and backslashes");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestAuthDefaultsAreConservative();
    passed &= TestSqlUpdatesAreFilteredAndSorted();
    passed &= TestEmptyAuthDatabaseInstallsBaseAndPendingUpdates();
    passed &= TestExistingAuthDatabaseSkipsAppliedUpdates();
    passed &= TestExistingAuthDatabaseRejectsChangedAppliedUpdate();
    passed &= TestExistingAuthDatabaseWithoutTrackingFailsSafe();
    passed &= TestExistingAuthDatabaseCanBaselineUpdates();
    passed &= TestExistingAuthDatabaseBaselineRequiresHashes();
    passed &= TestMissingDatabaseRequiresAutoCreate();
    passed &= TestSqlUpdatesAreDiscoveredFromConfiguredRoot();
    passed &= TestSqlScriptSplitsStatementsSafely();
    passed &= TestSqlScriptExecutorStopsOnFailure();
    passed &= TestStableSqlHash();
    passed &= TestSqlStringEscaping();

    return passed ? 0 : 1;
}
