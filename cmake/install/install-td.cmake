
function(install_td)
  if (WIN32 AND ${SPY_PACKAGED_BUILD})
    set(_td_cmake_modules ${CMAKE_SOURCE_DIR}/cmake/find/openssl)
  endif()

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
      -DCMAKE_MODULE_PATH=${_td_cmake_modules}
  )

endfunction()


## Function to include 
macro(include_td)
  set(Td_DIR ${CMAKE_BINARY_DIR}/external/td/install/lib/cmake/Td CACHE PATH "Tdlib prefix")

  find_package(Td CONFIG REQUIRED)

endmacro()

