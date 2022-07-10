
#ifndef spy_SwaggerComponent
#define spy_SwaggerComponent

#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>
#include <oatpp/core/macro/component.hpp>

#include <spy/version.h>

namespace spy {

    /**
     *  Swagger ui is served at
     *  http://host:port/swagger/ui
     */
    class SwaggerComponent {
    public:
  
        /**
         *  General API docs info
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {
            oatpp::swagger::DocumentInfo::Builder builder;

            builder
                .setTitle("Spy X telegram")
                .setDescription("Spy project backend application")
                .setVersion(SPY_VERSION_STRING)
                .setContactName("DeinsOne")
                .setContactUrl("https://github.com/DeinsOne/Spy-X-telegram")

                .addServer("http://localhost:8089", "server on localhost");

            return builder.build();

        }());

        /**
         *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
            return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
        }());
  
    };
} // namespace spy {

#endif // spy_SwaggerComponent
