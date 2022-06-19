
#ifndef tdlpp_LongPoll
#define tdlpp_LongPoll

#include <tdlpp/core.hpp>
#include <tdlpp/router/Router.hpp>
#include <tdlpp/base/Handler.hpp>

namespace tdlpp { namespace poll {

    class PollManager;

    // Class executing poll, performing program's flow 
    class LongPoll : public TdlppBase {
        friend poll::PollManager;
    public:
        /**
         * @brief A factory method
         * 
         * @return std::shared_ptr<LongPoll> New instance of 'LongPoll'
         */
        static std::shared_ptr<LongPoll> create(const std::shared_ptr<base::TdlppHandler>& handler);

    public:
        LongPoll(const std::shared_ptr<base::TdlppHandler>& handler);

        /**
         * @brief A factory method that executespolling thread and returns a new instance of 'PollManager'
         * 
         * @return std::shared_ptr<poll::PollManager> New instance of 'PollManager'
         */
        std::shared_ptr<poll::PollManager> ExecuteAsync();

    private:
        std::shared_ptr<poll::LongPoll> this_;
        std::shared_ptr<router::Router> router_;
        std::shared_ptr<base::TdlppHandler> handler_;
    };

} // namespace poll
} // namespace tdlpp

#endif // tdlpp_LongPoll
