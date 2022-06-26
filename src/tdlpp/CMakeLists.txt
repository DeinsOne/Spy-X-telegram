
find_package(Td REQUIRED NO_DEFAULT_PATH)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

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

target_include_directories(tdlpp PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../ ${CMAKE_BINARY_DIR})
target_link_libraries(tdlpp PUBLIC ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY} ${ZLIB_LIBRARIES})
target_link_libraries(tdlpp PUBLIC Td::TdStatic)


if (${SPY_ENABLE_OPTIMIZATION})
  target_compile_options(tdlpp PRIVATE -O3)
endif()


## Instalation
install(TARGETS tdlpp DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
if (${SPY_PACKAGED_BUILD})
  set_target_properties(tdlpp PROPERTIES INSTALL_RPATH .)
endif()
