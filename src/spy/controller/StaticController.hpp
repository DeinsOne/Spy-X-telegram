
#ifndef spy_StaticController
#define spy_StaticController

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)     //<- Begin Codegen

namespace spy { namespace controller {

    class StaticController : public oatpp::web::server::api::ApiController {
    public:
        StaticController(const std::shared_ptr<ObjectMapper>& objectMapper)
          : oatpp::web::server::api::ApiController(objectMapper)
        {}

        static std::shared_ptr<StaticController> createShared(
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) // Inject objectMapper component here as default parameter
        ) {
            return std::make_shared<StaticController>(objectMapper);
        }

    public:
        ENDPOINT("GET", "/", root) {
            const char* html =
                "<html lang='en'>"
                "  <head>"
                "    <meta charset=utf-8/>"
                "  </head>"
                "  <body>"
                "    <p>TelegramSpy root endpoint</p>"
                "    <a href='swagger/ui'>Checkout Swagger-UI page</a>"
                "  </body>"
                "</html>";
            auto response = createResponse(Status::CODE_200, html);
            response->putHeader(Header::CONTENT_TYPE, "text/html");
            return response;
        }

    };

} } // namespace spy { namespace controller

#include OATPP_CODEGEN_END(ApiController)       //<- End Codegen

#endif // spy_StaticController
