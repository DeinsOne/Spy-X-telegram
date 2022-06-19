
#ifndef spy_MessageContent
#define spy_MessageContent

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto { namespace messageContent {

    class MessageContent : public oatpp::DTO {
        DTO_INIT(MessageContent, DTO)

        DTO_FIELD(oatpp::Int64, id);
        DTO_FIELD(oatpp::Int64, chat_id);
        DTO_FIELD(oatpp::Int32, version_);
    };


} } } // namespace spy { namespace dto { namespace messageContent {

#include OATPP_CODEGEN_END(DTO)

#endif //spy_MessageContent
