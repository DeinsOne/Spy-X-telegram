
#ifndef spy_ChatsDatabase
#define spy_ChatsDatabase

#include <oatpp/core/macro/codegen.hpp>

#include <oatpp/orm/DbClient.hpp>
#include <oatpp/orm/SchemaMigration.hpp>
#include <oatpp-sqlite/orm.hpp>

#include <spy/utils/StringTools.h>
#include <spy/dto/Chat.hpp>

#include OATPP_CODEGEN_BEGIN(DbClient)      //<- Begin Codegen

namespace spy { namespace db {

    class ChatsDatabase : public oatpp::orm::DbClient {
    public:
        ChatsDatabase(const std::shared_ptr<oatpp::orm::Executor>& executor) : oatpp::orm::DbClient(executor) {
            oatpp::orm::SchemaMigration migration(executor);

            try {
                migration.addFile(1, DATABASE_MIGRATIONS "chats-migration.sql");
                migration.migrate();
            }
            catch (std::exception& err) {
                OATPP_LOGE("Migration failed", "ChatsDatabase: %s", err.what())
            }
        }

    public:
        void SetChatLastMessage(const std::int64_t& chatId, const std::int64_t& messageId) {
            auto transaction = beginTransaction();

            executeQuery("UPDATE chats SET lastMessageId=:id WHERE id=:chat_id;",
                {
                    { "id", oatpp::Int64((int)messageId) },
                    { "chat_id", oatpp::Int64((int)chatId) }
                }
            );

            transaction.commit();
        }

    public:
        QUERY(addChat,
            "INSERT INTO chats (id, title, lastMessageId, fetchSince, fetched) " \
            "VALUES (:chat.id, :chat.title, :chat.lastMessageId, :chat.fetchSince, :chat.fetched) " \
            "ON CONFLICT (id) DO UPDATE SET title = :chat.title;",
            PARAM(oatpp::Object<dto::Chat>, chat)
        );

        QUERY(updateChat,
            "UPDATE chats SET " \
            " id=:chat.id, " \
            " title=:chat.title, " \
            " lastMessageId=:chat.lastMessageId,"
            " fetchSince=:chat.fetchSince, " \
            " fetched=:chat.fetched " \
            "WHERE id=:chat.id;",
            PARAM(oatpp::Object<dto::Chat>, chat)
        );

        QUERY(getChats,
            "SELECT * FROM chats LIMIT :limit OFFSET :offset;",
            PARAM(oatpp::Int64, limit),
            PARAM(oatpp::Int64, offset)
        );

        QUERY(getChatById,
            "SELECT * FROM chats WHERE id=:id;",
            PARAM(oatpp::Int64, id)
        );

        QUERY(deleteChatById,
            "DELETE FROM chats WHERE id=:id;",
            PARAM(oatpp::Int64, id)
        );

    };

} } // namespace spy { namespace db {

#include OATPP_CODEGEN_END(DbClient)        //<- End Codegen

#endif // spy_ChatsDatabase
