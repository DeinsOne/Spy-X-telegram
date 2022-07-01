
#ifndef spy_SessionsDb
#define spy_SessionsDb

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include <oatpp-sqlite/orm.hpp>
#include <spy/utils/Logger/SpyLog.h>

#include OATPP_CODEGEN_BEGIN(DbClient)      //<- Begin Codegen

namespace spy { namespace db {

    class SessionsDatabase : public oatpp::orm::DbClient {
    public:
        SessionsDatabase(const std::shared_ptr<oatpp::orm::Executor>& executor) : oatpp::orm::DbClient(executor) {
            oatpp::orm::SchemaMigration migration(executor);

            try {
                migration.addFile(1, DATABASE_MIGRATIONS "sessions-migration.sql");
                migration.addFile(2, DATABASE_MIGRATIONS "permissions-migration.sql");
                migration.migrate();
            }
            catch (std::exception& err) {
                SPY_LOGE("SessionsDatabase:SessionsDatabase Migration failed -> %s", err.what());
            }
        }

    };

} } // namespace spy { namespace db

#include OATPP_CODEGEN_END(DbClient)        //<- End Codegen

#endif // spy_SessionsDb
