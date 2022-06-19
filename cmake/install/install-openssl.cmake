
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

  list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_BINARY_DIR}/external/openssl/install)

  file(GLOB_RECURSE openssl_libs
    "${OPENSSL_ROOT_DIR}/lib/*.so*"
    "${OPENSSL_ROOT_DIR}/bin/*.dll*"
  )

  install(FILES ${openssl_libs} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

endmacro()

