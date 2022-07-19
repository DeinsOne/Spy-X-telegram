
#ifndef spy_MessageModification
#define spy_MessageModification

#include <td/telegram/td_api.h>

#include <spy/dto/MessageContent.hpp>

#include <spy/dto/messageContent/MessageText.hpp>
#include <spy/dto/messageContent/MessagePhoto.hpp>
#include <spy/dto/messageContent/MessageVideo.hpp>
#include <spy/dto/messageContent/MessageVideoNote.hpp>
#include <spy/dto/messageContent/MessageVoiceNote.hpp>
#include <spy/dto/messageContent/MessageDocument.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto {

    ENUM(MessageContentType, v_int32,
        VALUE(deleted,                                  0, "deleted"),
        VALUE(messageAnimatedEmoji,                     td::td_api::messageAnimatedEmoji::ID, "messageAnimatedEmoji"),
        VALUE(messageAnimation,                         td::td_api::messageAnimation::ID, "messageAnimation"),
        VALUE(messageAudio,                             td::td_api::messageAudio::ID, "messageAudio"),
        VALUE(messageBasicGroupChatCreate,              td::td_api::messageBasicGroupChatCreate::ID, "messageBasicGroupChatCreate"),
        VALUE(messageCall,                              td::td_api::messageCall::ID, "messageCall"),
        VALUE(messageChatAddMembers,                    td::td_api::messageChatAddMembers::ID, "messageChatAddMembers"),
        VALUE(messageChatChangePhoto,                   td::td_api::messageChatChangePhoto::ID, "messageChatChangePhoto"),
        VALUE(messageChatChangeTitle,                   td::td_api::messageChatChangeTitle::ID, "messageChatChangeTitle"),
        VALUE(messageChatDeleteMember,                  td::td_api::messageChatDeleteMember::ID, "messageChatDeleteMember"),
        VALUE(messageChatDeletePhoto,                   td::td_api::messageChatDeletePhoto::ID, "messageChatDeletePhoto"),
        VALUE(messageChatJoinByLink,                    td::td_api::messageChatJoinByLink::ID, "messageChatJoinByLink"),
        VALUE(messageChatJoinByRequest,                 td::td_api::messageChatJoinByRequest::ID, "messageChatJoinByRequest"),
        VALUE(messageChatSetTheme,                      td::td_api::messageChatSetTheme::ID, "messageChatSetTheme"),
        VALUE(messageChatSetTtl,                        td::td_api::messageChatSetTtl::ID, "messageChatSetTtl"),
        VALUE(messageChatUpgradeFrom,                   td::td_api::messageChatUpgradeFrom::ID, "messageChatUpgradeFrom"),
        VALUE(messageChatUpgradeTo,                     td::td_api::messageChatUpgradeTo::ID, "messageChatUpgradeTo"),
        VALUE(messageContact,                           td::td_api::messageContact::ID, "messageContact"),
        VALUE(messageContactRegistered,                 td::td_api::messageContactRegistered::ID, "messageContactRegistered"),
        VALUE(messageCustomServiceAction,               td::td_api::messageCustomServiceAction::ID, "messageCustomServiceAction"),
        VALUE(messageDice,                              td::td_api::messageDice::ID, "messageDice"),
        VALUE(messageDocument,                          td::td_api::messageDocument::ID, "messageDocument"),
        VALUE(messageExpiredPhoto,                      td::td_api::messageExpiredPhoto::ID, "messageExpiredPhoto"),
        VALUE(messageExpiredVideo,                      td::td_api::messageExpiredVideo::ID, "messageExpiredVideo"),
        VALUE(messageGame,                              td::td_api::messageGame::ID, "messageGame"),
        VALUE(messageGameScore,                         td::td_api::messageGameScore::ID, "messageGameScore"),
        VALUE(messageInviteVideoChatParticipants,       td::td_api::messageInviteVideoChatParticipants::ID, "messageInviteVideoChatParticipants"),
        VALUE(messageInvoice,                           td::td_api::messageInvoice::ID, "messageInvoice"),
        VALUE(messageLocation,                          td::td_api::messageLocation::ID, "messageLocation"),
        VALUE(messagePassportDataReceived,              td::td_api::messagePassportDataReceived::ID, "messagePassportDataReceived"),
        VALUE(messagePassportDataSent,                  td::td_api::messagePassportDataSent::ID, "messagePassportDataSent"),
        VALUE(messagePaymentSuccessful,                 td::td_api::messagePaymentSuccessful::ID, "messagePaymentSuccessful"),
        VALUE(messagePaymentSuccessfulBot,              td::td_api::messagePaymentSuccessfulBot::ID, "messagePaymentSuccessfulBot"),
        VALUE(messagePhoto,                             td::td_api::messagePhoto::ID, "messagePhoto"),
        VALUE(messagePinMessage,                        td::td_api::messagePinMessage::ID, "messagePinMessage"),
        VALUE(messagePoll,                              td::td_api::messagePoll::ID, "messagePoll"),
        VALUE(messageProximityAlertTriggered,           td::td_api::messageProximityAlertTriggered::ID, "messageProximityAlertTriggered"),
        VALUE(messageScreenshotTaken,                   td::td_api::messageScreenshotTaken::ID, "messageScreenshotTaken"),
        VALUE(messageSticker,                           td::td_api::messageSticker::ID, "messageSticker"),
        VALUE(messageSupergroupChatCreate,              td::td_api::messageSupergroupChatCreate::ID, "messageSupergroupChatCreate"),
        VALUE(messageText,                              td::td_api::messageText::ID, "messageText"),
        VALUE(messageUnsupported,                       td::td_api::messageUnsupported::ID, "messageUnsupported"),
        VALUE(messageVenue,                             td::td_api::messageVenue::ID, "messageVenue"),
        VALUE(messageVideo,                             td::td_api::messageVideo::ID, "messageVideo"),
        VALUE(messageVideoChatEnded,                    td::td_api::messageVideoChatEnded::ID, "messageVideoChatEnded"),
        VALUE(messageVideoChatScheduled,                td::td_api::messageVideoChatScheduled::ID, "messageVideoChatScheduled"),
        VALUE(messageVideoChatStarted,                  td::td_api::messageVideoChatStarted::ID, "messageVideoChatStarted"),
        VALUE(messageVideoNote,                         td::td_api::messageVideoNote::ID, "messageVideoNote"),
        VALUE(messageVoiceNote,                         td::td_api::messageVoiceNote::ID, "messageVoiceNote"),
        VALUE(messageWebsiteConnected,                  td::td_api::messageWebsiteConnected::ID, "messageWebsiteConnected")
    )

    class MessageModificationContentDto : public oatpp::DTO {
        DTO_INIT(MessageModificationContentDto, DTO)

        DTO_FIELD(oatpp::Int32, version_);

        DTO_FIELD(oatpp::Enum<MessageContentType>::AsString, message_content);
        DTO_FIELD(oatpp::Any, content);
        DTO_FIELD(oatpp::String, date_);

        DTO_FIELD_TYPE_SELECTOR(content) {
            if(!message_content) return Void::Class::getType();
            switch (*message_content) {
                case MessageContentType::deleted: return nullptr;
                case MessageContentType::messageText: return oatpp::Object<messageContent::MessageTextDto>::Class::getType();
                case MessageContentType::messagePhoto: return oatpp::Object<messageContent::MessagePhotoDto>::Class::getType();
                case MessageContentType::messageVideo: return oatpp::Object<messageContent::MessageVideoDto>::Class::getType();
                case MessageContentType::messageVideoNote: return oatpp::Object<messageContent::MessageVideoNoteDto>::Class::getType();
                case MessageContentType::messageVoiceNote: return oatpp::Object<messageContent::MessageVoiceNoteDto>::Class::getType();
                case MessageContentType::messageDocument: return oatpp::Object<messageContent::MessageDocumentDto>::Class::getType();
            }

            return nullptr;
        }
    };

    class MessageModificationDto : public MessageModificationContentDto {
        DTO_INIT(MessageModificationDto, MessageModificationContentDto)

        DTO_FIELD(oatpp::Int64, id);
        DTO_FIELD(oatpp::Int64, chat_id);
    };


} } // namespace spy { namespace dto {

#include OATPP_CODEGEN_END(DTO)

#endif //spy_MessageModification
