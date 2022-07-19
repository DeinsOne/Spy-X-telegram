
#ifndef spy_SequenceDto
#define spy_SequenceDto

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto {

    template<typename T>
    class SequenceDto : public oatpp::DTO {
        DTO_INIT(SequenceDto, DTO)
        DTO_FIELD(oatpp::Vector<T>, sequence);
    };

} } // namespace spy { namespace dto {

#include OATPP_CODEGEN_END(DTO)

#endif // spy_SequenceDto
