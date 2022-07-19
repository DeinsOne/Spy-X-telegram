
function(install_plog)
  superbuild_subproject(
    NAME plog
    GIT "https://github.com/SergiusTheBest/plog.git"
    TAG "master"
    SOURCE_PATH ${CMAKE_SOURCE_DIR}/external/plog
    INSTALL_PATH ${CMAKE_BINARY_DIR}/external/plog/install
    BUILD_ARGS
      -DPLOG_BUILD_SAMPLES=OFF
      -DPLOG_INSTALL=ON
  )

endfunction()


## Function to include 
macro(include_plog)
  set(plog_DIR ${CMAKE_BINARY_DIR}/external/plog/install/lib/cmake/plog CACHE PATH "Plog prefix")

  find_package(plog REQUIRED NO_DEFAULT_PATH)

endmacro()

