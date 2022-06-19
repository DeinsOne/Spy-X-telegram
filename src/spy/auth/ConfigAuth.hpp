
#ifndef spy_ConfigAuth
#define spy_ConfigAuth

#include <spy/utils/mini/ini.h>
#include <mutex>

namespace spy { namespace auth {
    /**
     * @brief ConfigAuth provides interface to create< write and read auth configuration file
     */
    class ConfigAuth {
    public:
        static std::shared_ptr<ConfigAuth> create(const std::string& filename = "spy-config.ini");

    public:
        /**
         * @brief Constructor initializing "auth-config.ini" file
         */
        ConfigAuth(const std::string& filename = "spy-config.ini");


        std::string GetApiHash();
        std::int64_t GetApiId();
        std::string GetTdDatabaseDir();
        std::string GetTdLanguageCode();
        std::string GetTdDeviceModel();

        void SetApiHash(const std::string& apiHash);
        void SetApiId(const std::int64_t& apiId);
        void SetTdDatabaseDir(const std::string& dir);
        void SetTdLanguageCode(const std::string& code);
        void SetTdDeviceModel(const std::string& model);

    private:
        std::shared_ptr<mINI::INIFile> file;
        mINI::INIStructure data;

        std::mutex dataMutex;
    };

} } // namespace spy { namespace auth


#endif // spy_ConfigAuth
