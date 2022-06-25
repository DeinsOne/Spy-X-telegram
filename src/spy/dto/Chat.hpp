
#ifndef spy_Chat
#define spy_Chat

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto {

    class Chat : public oatpp::DTO {
        DTO_INIT(Chat, DTO)

        DTO_FIELD(oatpp::Int64, id);
        DTO_FIELD(oatpp::String, title);
        DTO_FIELD(oatpp::Int64, lastMessageId);
        DTO_FIELD(oatpp::String, fetchSince);
        DTO_FIELD(oatpp::String, fetched);
    };


} } // namespace spy { namespace dto {

#include OATPP_CODEGEN_END(DTO)

#endif //spy_Chat
