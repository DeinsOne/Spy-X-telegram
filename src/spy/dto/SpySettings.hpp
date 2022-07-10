
#ifndef spy_SpySettings
#define spy_SpySettings

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto {

    ENUM(ChatsGroupTypeOatpp, v_int32,
        VALUE(includeAll, 0, "include_all"),
        VALUE(excludeAll, 1, "exclude_all")
    )

    class SpySettingsDTO : public oatpp::DTO {
        DTO_INIT(SpySettingsDTO, DTO)

        DTO_FIELD(oatpp::Vector<oatpp::Int64>, includeChats);
        DTO_FIELD(oatpp::Vector<oatpp::Int64>, excludeChats);
        DTO_FIELD(oatpp::Enum<ChatsGroupTypeOatpp>::AsString, chatsGroupType)           = dto::ChatsGroupTypeOatpp::includeAll;

        DTO_FIELD(oatpp::Boolean, forceExcludeChannels)                                 = true;

        DTO_FIELD(oatpp::Boolean, saveDeletedMedia)                                     = true;
        DTO_FIELD(oatpp::Boolean, saveSecretContent)                                    = true;
        DTO_FIELD(oatpp::Float64, saveMediaSizeLimit)                                   = 20;
    };


} } // namespace spy { namespace dto {

#include OATPP_CODEGEN_END(DTO)

#endif //spy_SpySettings
