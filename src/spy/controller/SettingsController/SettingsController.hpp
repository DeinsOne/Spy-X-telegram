
#ifndef spy_SettingsControllerRest
#define spy_SettingsControllerRest

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>

#include <spy/dto/SpySettings.hpp>
#include <spy/dto/helpers/SequenceDto.hpp>
#include <spy/dto/helpers/StatusDto.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace spy { namespace controller {

    class SettingsController : public oatpp::web::server::api::ApiController {
    public:
        SettingsController(
            spy::service::controller::SpySettingsController* settingsControllerPointer,
            const std::shared_ptr<ObjectMapper>& objectMapper
        )
          : oatpp::web::server::api::ApiController(objectMapper), settingsControllerPointer_(settingsControllerPointer)
        {}

        static std::shared_ptr<SettingsController> createShared(
            spy::service::controller::SpySettingsController* settingsControllerPointer,
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)
        ) {
            return std::make_shared<SettingsController>(settingsControllerPointer, objectMapper);
        }

    public: // Chats endpoints

        ENDPOINT("POST", "/settings", SetChatsGroupType, QUERY(oatpp::String, group_type)) {
            OATPP_ASSERT_HTTP(settingsControllerPointer_ != nullptr, Status::CODE_500, "settingsControllerPointer_ is an unitialized pointer");

            if (group_type == "include_all") {
                settingsControllerPointer_->SetChatsGroupType(spy::service::controller::ChatsGroupType::include_all);
                return createResponse(Status::CODE_200, "Ok");
            }
            else if (group_type == "exclude_all") {
                settingsControllerPointer_->SetChatsGroupType(spy::service::controller::ChatsGroupType::exclude_all);
                return createResponse(Status::CODE_200, "Ok");
            }

            return createResponse(Status::CODE_400, "Bad group_type value");
        }

        ENDPOINT("POST", "/settings/exclude_chats", SetExcludedChats, BODY_DTO(oatpp::Object<dto::SequenceDto<oatpp::Int64>>, exclude)) {
            OATPP_ASSERT_HTTP(settingsControllerPointer_ != nullptr, Status::CODE_500, "settingsControllerPointer_ is an unitialized pointer");

            std::vector<std::int64_t> excludeChats;
            for (auto it = exclude->sequence->begin(); it != exclude->sequence->end(); ++it) {
                excludeChats.emplace_back(*it);
            }

            settingsControllerPointer_->SetExcludeChats(excludeChats);
            return createResponse(Status::CODE_200, "Ok");
        }

        ENDPOINT("POST", "/settings/include_chats", SetIncludeChats, BODY_DTO(oatpp::Object<dto::SequenceDto<oatpp::Int64>>, include)) {
            OATPP_ASSERT_HTTP(settingsControllerPointer_ != nullptr, Status::CODE_500, "settingsControllerPointer_ is an unitialized pointer");

            std::vector<std::int64_t> includeChats;
            for (auto it = include->sequence->begin(); it != include->sequence->end(); ++it) {
                includeChats.emplace_back(*it);
            }

            settingsControllerPointer_->SetIncludeChats(includeChats);
            return createResponse(Status::CODE_200, "Ok");
        }

    public: // Media endpoints

        ENDPOINT("POST", "/settings/save_deleted_media", SetSaveDeletedMedia, QUERY(oatpp::Boolean, save_deleted)) {
            OATPP_ASSERT_HTTP(settingsControllerPointer_ != nullptr, Status::CODE_500, "settingsControllerPointer_ is an unitialized pointer");
            settingsControllerPointer_->SetSaveDeletedMedia(*save_deleted);
            return createResponse(Status::CODE_200, "Ok");
        }

        ENDPOINT("POST", "/settings/save_secret_media", SetSaveSecretMedia, QUERY(oatpp::Boolean, save_secret)) {
            OATPP_ASSERT_HTTP(settingsControllerPointer_ != nullptr, Status::CODE_500, "settingsControllerPointer_ is an unitialized pointer");
            settingsControllerPointer_->SetSaveSecretContent(*save_secret);
            return createResponse(Status::CODE_200, "Ok");
        }

        ENDPOINT("POST", "/settings/save_media_limit", SetSaveMediaSizeLimit, QUERY(oatpp::Float64, size_limit)) {
            OATPP_ASSERT_HTTP(settingsControllerPointer_ != nullptr, Status::CODE_500, "settingsControllerPointer_ is an unitialized pointer");
            settingsControllerPointer_->SetSaveMediaSizeLimit(*size_limit);
            return createResponse(Status::CODE_200, "Ok");
        }

    public:

        ENDPOINT("GET", "/settings", GetSpySettings) {
            OATPP_ASSERT_HTTP(settingsControllerPointer_ != nullptr, Status::CODE_500, "settingsControllerPointer_ is an unitialized pointer");

            auto settingsDto = spy::dto::SpySettingsDTO::createShared();

            settingsDto->chatsGroupType = dto::ChatsGroupTypeOatpp((int)settingsControllerPointer_->GetChatsGroupType());
            settingsDto->forceExcludeChannels = settingsControllerPointer_->GetForceExcludeChannels();
            settingsDto->saveDeletedMedia = settingsControllerPointer_->GetSaveDeletedMedia();
            settingsDto->saveSecretContent = settingsControllerPointer_->GetSaveSecretContent();
            settingsDto->saveMediaSizeLimit = settingsControllerPointer_->GetSaveMediaSizeLimit();


            // Add excluded chats
            if (settingsControllerPointer_->GetExcludeChats().size() > 0) {
                settingsDto->excludeChats = oatpp::Vector<oatpp::Int64>::createShared();

                for (std::size_t i = 0; i < settingsControllerPointer_->GetExcludeChats().size(); i++) {
                    settingsDto->excludeChats->emplace_back(settingsControllerPointer_->GetExcludeChats().at(i));
                }
            }

            // Add included chats
            if (settingsControllerPointer_->GetIncludeChats().size() > 0) {
                settingsDto->includeChats = oatpp::Vector<oatpp::Int64>::createShared();

                for (std::size_t i = 0; i < settingsControllerPointer_->GetIncludeChats().size(); i++) {
                    settingsDto->includeChats->emplace_back(settingsControllerPointer_->GetIncludeChats().at(i));
                }
            }

            return createDtoResponse(Status::CODE_200, settingsDto);
        }

        ENDPOINT("POST", "/settings/exclude_channels", SetForceExcludeChannels, QUERY(oatpp::Boolean, exclude_channels)) {
            OATPP_ASSERT_HTTP(settingsControllerPointer_ != nullptr, Status::CODE_500, "settingsControllerPointer_ is an unitialized pointer");
            settingsControllerPointer_->SetForceExcludeChannels(*exclude_channels);
            return createResponse(Status::CODE_200, "Ok");
        }

    private:
        spy::service::controller::SpySettingsController* settingsControllerPointer_;

    };

} } // namespace spy { namespace controller

#include OATPP_CODEGEN_END(ApiController)

#endif // spy_SettingsControllerRest
