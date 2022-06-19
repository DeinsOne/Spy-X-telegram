
#ifndef spy_CmdParser
#define spy_CmdParser

#include <cxxopts.hpp>
#include <spy/version.h>

namespace spy { namespace utils {

    /**
     * @brief Singleton class responsible for parsing and distributing command line arguments
     * within each application runtime
     */
    class CmdParserSingleton final {
    public:
        /**
         * @brief Singleton pattern method
         * @return The only one instance of CmdParserSingleton
         */
        static CmdParserSingleton* Get() {
            static CmdParserSingleton* instance = new CmdParserSingleton;
            return instance;
        }

        /**
         * @brief Parsing command line arguments and taking reaction on help and version flags
         * 
         * @param argc Program's arguments count
         * @param argv Program's arguments array
         */
        void ParseArguments(int argc, char** argv) {
            result = options->parse(argc, argv);

            if (result.count("help")) {
                printf("%s\n", options->help().c_str());
                exit(0);
            }
            else if (result.count("version")) {
                printf("TelegramSpy v%s\n", SPY_VERSION_STRING);
                exit(0);
            }
        }

        /**
         * @brief Function that returns the value of the argument with the given key.
         * @tparam T bool, int, float, string, vector<any>
         * @param key Desired key name
         * @return T Value stored in given key
         */
        template<typename T>
        T GetArgument(const std::string& key) {
            return result[key].as<T>();
        }

    private:
        /**
         * @brief Singleton pattern constructor
         */
        CmdParserSingleton() {
            options = std::make_shared<cxxopts::Options>(
                "TelegramSpy",
                R"(
TelegramSpy is a powerful application which, at first, stores
edited, deleted and secret messages. It provides rest api to
allow external app to use telegram features
                )"
            );

            options->set_width(75);
            options->add_options()
                ("p,port",      "Rest client port",                     cxxopts::value<int>()->default_value("8089"))
                ("r,rest",      "Enable rest client",                   cxxopts::value<bool>()->default_value("true"))
                ("h,help",      "Telegram spy help information")
                ("v,version",   "Telegram spy version info");
        }

    private:
        std::shared_ptr<cxxopts::Options> options;
        cxxopts::ParseResult result;

    };

} } // namespace spy { namespace utils


#endif // spy_CmdParser
