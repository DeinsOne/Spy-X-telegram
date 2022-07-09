
function(install_zlib)
  superbuild_subproject(
    NAME zlib
    GIT "https://github.com/madler/zlib.git"
    TAG "master"
    SOURCE_PATH ${CMAKE_SOURCE_DIR}/external/zlib
    INSTALL_PATH ${CMAKE_BINARY_DIR}/external/zlib/install
    BUILD_ARGS
  )

endfunction()


## Function to include 
macro(include_zlib)
  set(ZLIB_ROOT ${CMAKE_BINARY_DIR}/external/zlib/install CACHE PATH "Openssl prefix")

  file(GLOB_RECURSE zlib_libs
    "${ZLIB_ROOT}/lib/*.so*"
    "${ZLIB_ROOT}/bin/*.dll*"
  )

  install(FILES ${zlib_libs} DESTINATION ${SPY_INSTALL_PACKAGED_DIR})

endmacro()

