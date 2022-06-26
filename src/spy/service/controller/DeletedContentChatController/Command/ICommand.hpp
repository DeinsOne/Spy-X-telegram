
#ifndef spy_DeletedContentChatCommand
#define spy_DeletedContentChatCommand

#include <string>

namespace spy { namespace service { namespace controller { namespace command {

    class ICommand {
    public:
        // True if command is copleted
        virtual bool IsDone() = 0;

        // Function to interrupt processing
        virtual void Cencel() = 0;

        // Function to process message
        virtual void Process(const std::string& commandLine) = 0;

        // Command to get inheritor class id
        virtual std::int64_t get_id() = 0;
    };

} } } }

#endif // spy_DeletedContentChatCommand
