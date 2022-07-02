#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>

#include <oatpp/core/Types.hpp>
#include <td/telegram/td_api.hpp>
#include <tdlpp/tdlpp.hpp>
#include <spy/utils/StringTools.h>

#include <spy/service/functions/Utils/FormattedTextToMarkdown.hpp>

#include <spy/service/contentWorker/ContentWorker.hpp>

void spy::db::MessagesDatabase::addBaseMessage(td::td_api::message& message) {
    // Declare raw parameters
    std::unordered_map<oatpp::String, oatpp::Void> params;

    // Find message's sender id
    td::td_api::downcast_call(*message.sender_id_, tdlpp::overloaded(
        [&](td::td_api::messageSenderChat& chat) {
            params.insert({ "sender_id", oatpp::Int64((int)chat.chat_id_) });
        },
        [&](td::td_api::messageSenderUser& user) {
            params.insert({ "sender_id", oatpp::Int64((int)user.user_id_) });
        },
        [](auto&) {}
    ));

    // Define raw parameters
    params.insert({ "id", oatpp::Int64((int)message.id_) });
    params.insert({ "chat_id", oatpp::Int64((int)message.chat_id_) });
    params.insert({ "media_album_id", oatpp::Int64((int)message.media_album_id_) });
    params.insert({ "can_be_deleted_for_all", oatpp::Boolean(message.can_be_deleted_for_all_users_) });

    // Declare base message in database
    auto result = executeQuery("INSERT INTO messages (id, sender_id, chat_id, media_album_id, can_be_deleted_for_all) VALUES " \
        "(:id, :sender_id, :chat_id, :media_album_id, :can_be_deleted_for_all);",
        params
    );
}

void spy::db::MessagesDatabase::addMessageModification(const std::int64_t& message_id, const std::int64_t& chat_id, const std::int32_t& version, td::td_api::MessageContent& content) {
    // Declare raw parameters
    std::unordered_map<oatpp::String, oatpp::Void> params;

    // Define raw parameters
    params.insert({ "id", oatpp::Int64((int)message_id) });
    params.insert({ "chat_id", oatpp::Int64((int)chat_id) });
    params.insert({ "version_", oatpp::Int64((int)version) });
    params.insert({ "message_content", oatpp::String(TDLPP_TD_ID_NAME(content.get_id())) });
    params.insert({ "date_", oatpp::String(StringTools::currentDatTime()) });

    // Declare modification in database
    auto result = executeQuery("INSERT INTO modifications (id, chat_id, version_, message_content, date_) VALUES " \
        "(:id, :chat_id, :version_, :message_content, :date_);",
        params
    );

    // Declare message's content in database
    addMessageContent(message_id, chat_id, version, content);
}

void spy::db::MessagesDatabase::addDeletedModification(const std::int64_t& message_id, const std::int64_t& chat_id) {
    // Get all message's modifications
    auto messageModifications = executeQuery("SELECT * FROM modifications WHERE id=:id AND chat_id=:chat_id ORDER BY version_ ASC;", 
        {
            { "id", oatpp::Int64((int)message_id) },
            { "chat_id", oatpp::Int64((int)chat_id) }
        }
    );

    auto modifications = messageModifications->fetch<oatpp::Vector<oatpp::Object<spy::dto::MessageModificationDto>>>();

    // Find the next version of the modification
    auto version = (modifications->size() > 0 ? modifications->size() : 0);

    // Declare modification in database
    auto result = executeQuery("INSERT INTO modifications (id, chat_id, version_, message_content, date_) VALUES " \
        "(:id, :chat_id, :version_, :message_content, :date_);",
        {
            { "id", oatpp::Int64((int)message_id) },
            { "chat_id", oatpp::Int64((int)chat_id) },
            { "version_", oatpp::Int64((int)version) },
            { "message_content", oatpp::String("deleted") },
            { "date_", oatpp::String(StringTools::currentDatTime()) }
        }
    );
}

void spy::db::MessagesDatabase::addMessageContent(const std::int64_t& message_id, const std::int64_t& chat_id, const std::int32_t& version, td::td_api::MessageContent& content) {
    auto worker = std::make_shared<service::content::ContentWorkerFactory>(content, message_id, chat_id);

    // Declare message's content in database
    worker->AddToDatabase(content, version);
}


void spy::db::MessagesDatabase::AddMessage(td::td_api::message& message) {
    // Begin database transaction
    auto transaction = beginTransaction();

    // At first write message's modifications. It will make final message accessible immediately
    addMessageModification((int)message.id_, (int)message.chat_id_, 0, *message.content_);

    // Write base message. From this moment message is accessible
    addBaseMessage(message);

    // Commit transaction to database
    transaction.commit();
}

void spy::db::MessagesDatabase::AddMessageModification(const std::int64_t& message_id, const std::int64_t& chat_id, td::td_api::MessageContent& content) {
    // Begin database transaction
    auto transaction = beginTransaction();

    // Get all message's modifications
    auto messageModifications = executeQuery("SELECT * FROM modifications WHERE id=:id AND chat_id=:chat_id ORDER BY version_ ASC;", 
        {
            { "id", oatpp::Int64((int)message_id) },
            { "chat_id", oatpp::Int64((int)chat_id) }
        }
    );

    auto modifications = messageModifications->fetch<oatpp::Vector<oatpp::Object<spy::dto::MessageModificationDto>>>();

    // Find the next version of the modification
    auto version = (modifications->size() > 0 ? modifications->size() : 0);

    // Declare modification in database
    addMessageModification((int)message_id, (int)chat_id, version, content);

    // Commit transaction to database
    transaction.commit();
}

void spy::db::MessagesDatabase::AddDeletedModifications(const std::vector<std::int64_t>& message_ids, const std::int64_t& chat_id) {
    // Begin database transaction
    auto transaction = beginTransaction();

    for (auto message_id : message_ids) {
        addDeletedModification((int)message_id, (int)chat_id);
    }

    // Commit transaction to database
    transaction.commit();
}

void spy::db::MessagesDatabase::AddFile(const std::string& id, const std::string& path, const std::size_t& size) {
    // Begin database transaction
    auto transaction = beginTransaction();

    // Declare file in the database
    auto result = executeQuery(
        "INSERT INTO files (id_, path, size, date) VALUES (:id, :path, :size, :date);",
        {
            { "id", oatpp::String(id) },
            { "path", oatpp::String(path) },
            { "size", oatpp::Int64(size) },
            { "date", oatpp::String(StringTools::currentDatTime()) }
        }
    );

    if (!result->isSuccess()) {
        SPY_LOGE("MessagesDatabase:AddFile: %s", result->getErrorMessage()->c_str());
    }

    // Commit transaction to database
    transaction.commit();
}
