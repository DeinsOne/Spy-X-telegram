
function(install_cxxopts)
  superbuild_subproject(
    NAME cxxopts
    GIT "https://github.com/jarro2783/cxxopts.git"
    TAG "master"
    SOURCE_PATH ${CMAKE_SOURCE_DIR}/external/cxxopts
    INSTALL_PATH ${CMAKE_BINARY_DIR}/external/cxxopts/install
    BUILD_ARGS
      -DCXXOPTS_BUILD_EXAMPLES=OFF
      -DCXXOPTS_BUILD_TESTS=OFF
      -DCXXOPTS_ENABLE_INSTALL=ON
  )

endfunction()


## Function to include 
macro(include_cxxopts)
  set(cxxopts_DIR ${CMAKE_BINARY_DIR}/external/cxxopts/install/lib/cmake/cxxopts CACHE PATH "Cxxopts prefix")

  find_package(cxxopts REQUIRED NO_DEFAULT_PATH)

endmacro()

