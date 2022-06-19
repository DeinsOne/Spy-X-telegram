
#ifndef tdlpp_PollManager
#define tdlpp_PollManager

#include <tdlpp/poll/LongPoll.hpp>

#include <condition_variable>
#include <thread>

namespace tdlpp { namespace poll {

    // PollManager is responsible for managing polling flow. Provides appropriate methods
    class PollManager : public TdlppBase {
    public:
        /**
         * @brief A factory method
         * 
         * @return std::shared_ptr<PollManager> New instance of 'PollManager'
         */
        static std::shared_ptr<PollManager> create(const std::shared_ptr<LongPoll>& poll);

    public:
        PollManager(const std::shared_ptr<LongPoll>& longPoll);

        // Waiting for the thread to finish
        ~PollManager();

        // Resuming the polling
        void Resume();
        // Pausing the polling
        void Pause();

        // Checking if the polling is active or not
        bool IsActive() { return active; }

    private:
        bool active;
        std::thread worker;

        bool destroy;
        std::condition_variable destroyLock;

        std::shared_ptr<LongPoll> poll;

        /**
         * @brief A callback function that is called when a response is received
         * 
         * @param response Response to handle
         */
        void handlingResponse(td::ClientManager::Response response);
    };


} // namespace base
} // namespace tdlpp

#endif // tdlpp_PollManager
