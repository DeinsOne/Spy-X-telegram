
#ifndef tdlpp_Router
#define tdlpp_Router

#include <tdlpp/core.hpp>
#include <mutex>

namespace tdlpp { namespace router {

    class Router : public TdlppBase {
    public:
        /**
         * @brief A factory method
         * 
         * @return std::shared_ptr<Router> New instance of 'Router'
         */
        static std::shared_ptr<Router> create();

    public:
        Router();

        /**
         * @brief Receiving a response from the td client manager
         * 
         * @param secondsTimeout Maximum time waiting for response in seconds 
         * @return td::ClientManager::Response Received response
         */
        td::ClientManager::Response Receive(const double& secondsTimeout = 0);

        /**
         * @brief Sending a request to the rd client manager
         * 
         * @param function 
         * @return td::ClientManager::RequestId Id of sent request
         */
        td::ClientManager::RequestId Send(UniqueObjectPtr<td::td_api::Function> function);

        /**
         * @brief Returning the id of last called request
         * 
         * @return td::ClientManager::RequestId 
         */
        td::ClientManager::RequestId GetLastRequestId() const { return requestId; }

    private:
        std::shared_ptr<td::ClientManager> clientManager;
        td::ClientManager::ClientId clientId{0};
        td::ClientManager::RequestId requestId{0};

        std::mutex writingLock;

        // A private method that returns the next request id
        td::ClientManager::RequestId _getRequestId() { return ++requestId; }
    };

} // namespace router
} // namespace tdlpp

#endif // tdlpp_Router
