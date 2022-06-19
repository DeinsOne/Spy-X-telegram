
function(install_td)
  superbuild_subproject(
    NAME td
    GIT "https://github.com/tdlib/td.git"
    TAG "master"
    SOURCE_PATH ${CMAKE_SOURCE_DIR}/external/td
    INSTALL_PATH ${CMAKE_BINARY_DIR}/external/td/install
    BUILD_ARGS
      -DGPERF_EXECUTABLE:FILEPATH=${GPERF_EXECUTABLE}
      -DZLIB_ROOT=${ZLIB_ROOT}
      -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
  )

endfunction()


## Function to include 
macro(include_td)
  set(Td_DIR ${CMAKE_BINARY_DIR}/external/td/install/lib/cmake/Td CACHE PATH "Tdlib prefix")

  list(APPEND CMAKE_PREFIX_PATH ${Td_DIR})
  list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/external/td/install)

  find_package(Td REQUIRED NO_DEFAULT_PATH)

endmacro()

