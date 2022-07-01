#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>

#include <oatpp/core/Types.hpp>
#include <td/telegram/td_api.hpp>
#include <tdlpp/tdlpp.hpp>
#include <spy/utils/StringTools.h>

#include <spy/service/functions/Utils/FormattedTextToMarkdown.hpp>

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
    // Declare message's content in database
    td::td_api::downcast_call(content, tdlpp::overloaded(
        [&](td::td_api::messageText& message) {
            spy::service::functions::FormattedTextToMarkdown parser(*message.text_);
            parser.Execute();

            auto result = executeQuery("INSERT INTO messageText (id, chat_id, version_, text_) VALUES " \
                "(:id, :chat_id, :version, :text);",
                {
                    { "id", oatpp::Int64(message_id) },
                    { "chat_id", oatpp::Int64(chat_id) },
                    { "version", oatpp::Int64(version) },
                    { "text", oatpp::String(parser.GetMarkdown()) }
                }
            );

            if (!result->isSuccess()) {
                SPY_LOGE("MessagesDatabase:addMessageContent(messageText): %s", result->getErrorMessage()->c_str());
            }
        },
        [&](td::td_api::messagePhoto& message) {
            spy::service::functions::FormattedTextToMarkdown parser(*message.caption_);
            parser.Execute();

            auto result = executeQuery(
                "INSERT INTO messagePhoto (id, chat_id, version_, caption_, is_secret_, file_id_, width, height) VALUES " \
                "(:id, :chat_id, :version, :caption, :secret, :file_id, :w, :h);",
                {
                    { "id", oatpp::Int64(message_id) },
                    { "chat_id", oatpp::Int64(chat_id) },
                    { "version", oatpp::Int64(version) },
                    { "caption", oatpp::String(parser.GetMarkdown()) },
                    { "secret", oatpp::Boolean(message.is_secret_) },
                    { "file_id", oatpp::String(message.photo_->sizes_.back()->photo_->remote_->unique_id_) },
                    { "w", oatpp::Int32((int)message.photo_->sizes_.back()->width_) },
                    { "h", oatpp::Int32((int)message.photo_->sizes_.back()->height_) }
                }
            );

            if (!result->isSuccess()) {
                SPY_LOGE("MessagesDatabase:addMessageContent(messagePhoto): %s", result->getErrorMessage()->c_str());
            }
        },
        [&](td::td_api::messageVideo& message) {
            spy::service::functions::FormattedTextToMarkdown parser(*message.caption_);
            parser.Execute();

            auto result = executeQuery(
                "INSERT INTO messageVideo (id, chat_id, version_, caption_, is_secret_, file_id_, duration, width, height, mime_type) VALUES " \
                "(:id, :chat_id, :version, :caption, :secret, :file_id, :d, :w, :h, :mime);",
                {
                    { "id", oatpp::Int64(message_id) },
                    { "chat_id", oatpp::Int64(chat_id) },
                    { "version", oatpp::Int64(version) },
                    { "caption", oatpp::String(parser.GetMarkdown()) },
                    { "secret", oatpp::Boolean(message.is_secret_) },
                    { "file_id", oatpp::String(message.video_->video_->remote_->unique_id_) },
                    { "d", oatpp::Int32((int)message.video_->duration_) },
                    { "w", oatpp::Int32((int)message.video_->width_) },
                    { "h", oatpp::Int32((int)message.video_->height_) },
                    { "mime", oatpp::String(message.video_->mime_type_) }
                }
            );

            if (!result->isSuccess()) {
                SPY_LOGE("MessagesDatabase:addMessageContent(messageVideo): %s", result->getErrorMessage()->c_str());
            }
        },
        [&](td::td_api::messageVoiceNote& message) {
            spy::service::functions::FormattedTextToMarkdown parser(*message.caption_);
            parser.Execute();

            auto result = executeQuery(
                "INSERT INTO messageVoiceNote (id, chat_id, version_, is_listened, file_id_, duration, mime_type) VALUES " \
                "(:id, :chat_id, :version, :listened, :file_id, :d, :mime);",
                {
                    { "id", oatpp::Int64(message_id) },
                    { "chat_id", oatpp::Int64(chat_id) },
                    { "version", oatpp::Int64(version) },
                    { "listened", oatpp::Boolean(message.is_listened_) },
                    { "file_id", oatpp::String(message.voice_note_->voice_->remote_->unique_id_) },
                    { "d", oatpp::Int32((int)message.voice_note_->duration_) },
                    { "mime", oatpp::String(message.voice_note_->mime_type_) }
                }
            );

            if (!result->isSuccess()) {
                SPY_LOGE("MessagesDatabase:addMessageContent(messageVoiceNote): %s", result->getErrorMessage()->c_str());
            }
        },
        [&](td::td_api::messageVideoNote& message) {
            auto result = executeQuery(
                "INSERT INTO messageVideoNote (id, chat_id, version_, is_secret_, is_viewed, file_id_, duration, length) VALUES " \
                "(:id, :chat_id, :version, :secret, :viewed, :file_id, :d, :l);",
                {
                    { "id", oatpp::Int64(message_id) },
                    { "chat_id", oatpp::Int64(chat_id) },
                    { "version", oatpp::Int64(version) },
                    { "secret", oatpp::Boolean(message.is_secret_) },
                    { "viewed", oatpp::Boolean(message.is_viewed_) },
                    { "file_id", oatpp::String(message.video_note_->video_->remote_->unique_id_) },
                    { "d", oatpp::Int32((int)message.video_note_->duration_) },
                    { "l", oatpp::Int32((int)message.video_note_->length_) }
                }
            );

            if (!result->isSuccess()) {
                SPY_LOGE("MessagesDatabase", "addMessageContent(messageVideoNote): %s", result->getErrorMessage()->c_str());
            }
        },
        [&](td::td_api::messageDocument& message) {
            spy::service::functions::FormattedTextToMarkdown parser(*message.caption_);
            parser.Execute();

            auto result = executeQuery(
                "INSERT INTO messageDocument (id, chat_id, version_, caption, file_id_, mime_type) VALUES " \
                "(:id, :chat_id, :version, :caption, :file_id, :mime);",
                {
                    { "id", oatpp::Int64(message_id) },
                    { "chat_id", oatpp::Int64(chat_id) },
                    { "version", oatpp::Int64(version) },
                    { "caption", oatpp::String(parser.GetMarkdown()) },
                    { "file_id", oatpp::String(message.document_->document_->remote_->unique_id_) },
                    { "mime", oatpp::String(message.document_->mime_type_) }
                }
            );

            if (!result->isSuccess()) {
                SPY_LOGE("MessagesDatabase", "addMessageContent(messageVoiceNote): %s", result->getErrorMessage()->c_str());
            }
        },
        [&](auto&) {}
    ));
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
        SPY_LOGE("MessagesDatabase", "AddFile: %s", result->getErrorMessage()->c_str());
    }

    // Commit transaction to database
    transaction.commit();
}
