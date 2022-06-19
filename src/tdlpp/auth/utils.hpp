
#ifndef tdlpp_Utils
#define tdlpp_Utils

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__)
    #include <termios.h>
    #include <unistd.h>
    #include <stdio.h>
#endif

#include <iostream>
#include <string>

namespace tdlpp { namespace utils {

    inline std::string getpass(const char *prompt, const bool& show_asterisk = true);

#ifdef _WIN32
    inline std::string getpass(const char *prompt, const bool& show_asterisk) {
        const char BACKSPACE = 8;
        const char RETURN = 13;
        std::string password;
        unsigned char ch = 0;
        std::cout << prompt;
        DWORD con_mode;
        DWORD dwRead;
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(hIn, &con_mode );
        SetConsoleMode(hIn, con_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT) );
        while(ReadConsoleA(hIn, &ch, 1, &dwRead, NULL) && ch != RETURN) {
            if(ch == BACKSPACE) {
                if(password.length() != 0) {
                    if (show_asterisk)
                        std::cout << "\b \b";
                    password.resize(password.length() - 1);
                }
            }
            else {
                password += ch;
                if (show_asterisk)
                    std::cout << '*';
            }
        }
        std::cout << std::endl;
        SetConsoleMode(hIn, con_mode | ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
        return password;
    }
#elif defined(__linux__)
    // A function that gets a single character from the terminal without echoing it.
    inline int getch() {
        int ch;
        struct termios t_old, t_new;
        tcgetattr(STDIN_FILENO, &t_old);
        t_new = t_old;
        t_new.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &t_new);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
        return ch;
    }

    inline std::string getpass(const char *prompt, const bool& show_asterisk) {
        const char BACKSPACE = 127;
        const char RETURN = 10;
        std::string password;
        unsigned char ch=0;
        std::cout << prompt;
        while ((ch = getch()) != RETURN) {
            if (ch == BACKSPACE) {
                if (password.length() != 0) {
                    if (show_asterisk)
                        std::cout << "\b \b";
                    password.resize(password.length() - 1);
                }
            }
            else {
                password += ch;
                if(show_asterisk)
                    std::cout << '*';
            }
        }
        std::cout << std::endl;
        return password;
    }
#endif


} // namespace utils
} // namespace tdlpp

#endif // tdlpp_Utils
