
function(install_openssl)
  superbuild_subproject(
    NAME openssl
    GIT "https://github.com/janbar/openssl-cmake.git"
    TAG "master"
    SOURCE_PATH ${CMAKE_SOURCE_DIR}/external/openssl
    INSTALL_PATH ${CMAKE_BINARY_DIR}/external/openssl/install
    BUILD_ARGS
      -DBUILD_SHARED_LIBS=ON
  )

endfunction()


## Function to include 
macro(include_openssl)
  set(OPENSSL_ROOT_DIR ${CMAKE_BINARY_DIR}/external/openssl/install CACHE PATH "Openssl prefix")

  ## Windows only
  if (WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/find/openssl)
  endif()

  find_package(OpenSSL MODULE REQUIRED)

  file(GLOB_RECURSE openssl_libs
    "${OPENSSL_ROOT_DIR}/lib/*.so*"
    "${OPENSSL_ROOT_DIR}/bin/*.dll*"
  )

  install(FILES ${openssl_libs} DESTINATION ${SPY_INSTALL_PACKAGED_DIR})

endmacro()

