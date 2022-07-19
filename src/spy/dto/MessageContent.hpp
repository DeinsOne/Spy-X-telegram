
#ifndef spy_MessageContentDto
#define spy_MessageContentDto

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto { namespace messageContent {

    template<typename TContent>
    class MessageContentDto : public TContent {
        DTO_INIT(MessageContentDto, TContent)

        DTO_FIELD(oatpp::Int64, id);
        DTO_FIELD(oatpp::Int64, chat_id);
        DTO_FIELD(oatpp::Int32, version_);
    };


} } } // namespace spy { namespace dto { namespace messageContent {

#include OATPP_CODEGEN_END(DTO)

#endif // spy_MessageContentDto
