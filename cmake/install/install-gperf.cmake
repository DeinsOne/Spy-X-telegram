
function(install_gperf)
  superbuild_subproject(
    NAME gperf
    GIT "https://github.com/jwinarske/gperf.git"
    TAG "cmake"
    SOURCE_PATH ${CMAKE_SOURCE_DIR}/external/gperf
    INSTALL_PATH ${CMAKE_BINARY_DIR}/external/gperf/install
    BUILD_ARGS
  )

endfunction()


## Function to include 
macro(include_gperf)
  set(gperf_ROOT ${CMAKE_BINARY_DIR}/external/gperf/install)
  set(gperf_DIR ${CMAKE_BINARY_DIR}/external/gperf/install)

  find_program(GPERF_EXECUTABLE gperf PATHS
    ${CMAKE_BINARY_DIR}/external/gperf/install
    ${CMAKE_BINARY_DIR}/external/gperf/install/bin
  )

endmacro()

