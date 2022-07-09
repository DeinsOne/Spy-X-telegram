
find_package(Td REQUIRED NO_DEFAULT_PATH)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(plog REQUIRED)

# Generate bindings header
add_subdirectory(generator)


add_library(tdlpp SHARED
  auth/DefaultAuth.cpp
  base/BindingHandler.cpp
  base/UpdateCalbacksHandler.cpp
  base/Handler.cpp
  poll/Poll.cpp
  router/Router.cpp
  ${TDLPP_BINDING_HEADER}
)
add_dependencies(tdlpp binding_generator)

target_compile_definitions(tdlpp
  ## Enable tdlpp file logging
  PRIVATE -DTDLPP_ENABLE_FILE_LOGGING

  ## Define INFO file logging level
  PRIVATE -DTDLPP_FILE_LOGGING_LEVEL=5

  ## Enable td objects capturing 
  PRIVATE -DTDLPP_ENABLE_OBJECT_LOGGING

  ## TODO: Remove object logging from release build. Defines 8mb objects log file
  PRIVATE -DTDLPP_OBJECT_LOGGING_MAX_FILE_SIZE=8388608
)

target_include_directories(tdlpp PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../ ${CMAKE_BINARY_DIR})
target_link_libraries(tdlpp PUBLIC ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY} ${ZLIB_LIBRARIES})
target_link_libraries(tdlpp PUBLIC Td::TdStatic PUBLIC plog::plog)

target_compile_options(tdlpp PRIVATE ${SPY_COMPILE_OPTIONS})


install(TARGETS tdlpp DESTINATION ${SPY_INSTALL_PACKAGED_DIR})
if (${SPY_PACKAGED_BUILD})
  set_target_properties(tdlpp PROPERTIES INSTALL_RPATH .)
endif()
