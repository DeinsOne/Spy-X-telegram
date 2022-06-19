
#ifndef tdlpp_Core_hpp
#define tdlpp_Core_hpp

#if !defined(TDLPP_MAX_AUTH_RETRIES)
    #define TDLPP_MAX_AUTH_RETRIES 5
#endif

// Object logging provides storing incoming responses and outcoming requests of tdlib
// with rolling file logger to several separated files
#if defined(TDLPP_ENABLE_OBJECT_LOGGING) || defined(TDLPP_OBJECT_LOGGING_MAX_FILE_SIZE) || defined(TDLPP_OBJECT_LOGGING_MAX_FILES)
    #ifndef TDLPP_OBJECT_LOGGING_MAX_FILE_SIZE
        #define TDLPP_OBJECT_LOGGING_MAX_FILE_SIZE 1024 * 1024 * 16
    #endif

    #ifndef TDLPP_OBJECT_LOGGING_MAX_FILES
        #define TDLPP_OBJECT_LOGGING_MAX_FILES 1
    #endif

    #if !defined(TDLPP_ENABLE_OBJECT_LOGGING)
        #define TDLPP_ENABLE_OBJECT_LOGGING
    #endif
#else
    #define TDLPP_DISABLE_OBJECT_LOGGING
#endif

// Console logger outputs all allowed information to console using plot ColorConsoleAppender
#if defined(TDLPP_ENABLE_CONSOLE_LOGGING) || defined(TDLPP_CONSOLE_LOGGING_LEVEL)
    #if !defined(TDLPP_CONSOLE_LOGGING_LEVEL) && defined(TDLPP_LOGGING_LEVEL_COMMON)
        #define TDLPP_CONSOLE_LOGGING_LEVEL TDLPP_LOGGING_LEVEL_COMMON
    #elif !defined(TDLPP_CONSOLE_LOGGING_LEVEL)
        #define TDLPP_CONSOLE_LOGGING_LEVEL 3
    #endif

    #if !defined(TDLPP_ENABLE_CONSOLE_LOGGING)
        #define TDLPP_ENABLE_CONSOLE_LOGGING
    #endif
#else
    #define TDLPP_DISABLE_CONSOLE_LOGGING
#endif

// File logging involves storing of console output with enumerated separated files
#if defined(TDLPP_ENABLE_FILE_LOGGING) || defined(TDLPP_FILE_LOGGING_LEVEL) || defined(TDLPP_FILE_LOGGING_MAX_FILE_SIZE) || defined(TDLPP_FILE_LOGGING_MAX_FILES)
    #if !defined(TDLPP_FILE_LOGGING_LEVEL) && defined(TDLPP_LOGGING_LEVEL_COMMON)
        #define TDLPP_FILE_LOGGING_LEVEL TDLPP_LOGGING_LEVEL_COMMON
    #elif !defined(TDLPP_FILE_LOGGING_LEVEL)
        #define TDLPP_FILE_LOGGING_LEVEL 5
    #endif
    #if !defined(TDLPP_ENABLE_FILE_LOGGING)
        #define TDLPP_ENABLE_FILE_LOGGING
    #endif

    #ifndef TDLPP_FILE_LOGGING_MAX_FILE_SIZE
        #define TDLPP_FILE_LOGGING_MAX_FILE_SIZE 1024 * 1024 * 32
    #endif

    #ifndef TDLPP_FILE_LOGGING_MAX_FILES
        #define TDLPP_FILE_LOGGING_MAX_FILES 1
    #endif
#else
    #define TDLPP_DISABLE_FILE_LOGGING
#endif

#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>
#include <td/telegram/Client.h>

#include <memory>

#include <stdexcept>
#include <tdlpp/log/Logger.hpp>


namespace tdlpp {

    template<typename T>
    using UniqueObjectPtr = td::td_api::object_ptr<T>;

    template<typename T>
    using SharedObjectPtr = std::shared_ptr<T>;


    // A base class for all tdlpp classes
    struct TdlppBase { };

    // overloaded
    namespace detail {
        template <class... Fs>
        struct overload;

        template <class F>
        struct overload<F> : public F {
            explicit overload(F f) : F(f) {}
        };

        template <class F, class... Fs>
        struct overload<F, Fs...> : public overload<F>, public overload<Fs...> {
            overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...) {
            }
            using overload<F>::operator();
            using overload<Fs...>::operator();
        };

    } // namespace detail

    template <class... F>
    auto overloaded(F... f) {
        return detail::overload<F...>(f...);
    }

    template<typename TdObject>
    TdObject cast_object(SharedObjectPtr<td::td_api::Object> object) {
        TdObject casted;

        td::td_api::downcast_call(*object, overloaded(
            [&](TdObject& obj) { casted = std::move(obj); },
            [&](auto&) {
                TDLPP_LOG_ERROR("Cannot cast object '%s' to '%s'", TDLPP_TD_ID_NAME(object->get_id()), TDLPP_TD_ID_NAME(TdObject::ID));
                throw std::invalid_argument("tdlpp bad cast_object '" + log::__IdNameBinding(object->get_id()) + "' to '" + log::__IdNameBinding(TdObject::ID) + "'");
            }
        ));

        return std::move(casted);
    }
} // namespace tdlpp

#endif // tdlpp_Core_hpp
