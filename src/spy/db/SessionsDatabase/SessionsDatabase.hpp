
#ifndef spy_SessionsDb
#define spy_SessionsDb

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include <oatpp-sqlite/orm.hpp>

#include OATPP_CODEGEN_BEGIN(DbClient)      //<- Begin Codegen

namespace spy { namespace db {

    class SessionsDatabase : public oatpp::orm::DbClient {
    public:
        SessionsDatabase(const std::shared_ptr<oatpp::orm::Executor>& executor) : oatpp::orm::DbClient(executor) {
            oatpp::orm::SchemaMigration migration(executor);

            migration.addFile(1 /* start from version 1 */, DATABASE_MIGRATIONS "sessions-migration.sql");
            migration.addFile(2, DATABASE_MIGRATIONS "permissions-migration.sql");

            migration.migrate();        // <-- run migrations. This guy will throw on error.

            auto version = executor->getSchemaVersion();
            // OATPP_LOGD("SessionsDb", "Migration - OK. Version=%lld.", version);
        }

    };

} } // namespace spy { namespace db

#include OATPP_CODEGEN_END(DbClient)        //<- End Codegen

#endif // spy_SessionsDb
