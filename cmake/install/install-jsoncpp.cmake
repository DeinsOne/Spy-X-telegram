
function(install_jsoncpp)
  superbuild_subproject(
    NAME jsoncpp
    GIT "https://github.com/open-source-parsers/jsoncpp.git"
    TAG "master"
    SOURCE_PATH ${CMAKE_SOURCE_DIR}/external/jsoncpp
    INSTALL_PATH ${CMAKE_BINARY_DIR}/external/jsoncpp/install
    BUILD_ARGS
      -DJSONCPP_WITH_TESTS=OFF
      -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF
  )

endfunction()


## Function to include 
macro(include_jsoncpp)
  set(jsoncpp_DIR ${CMAKE_BINARY_DIR}/external/jsoncpp/install/lib/cmake/jsoncpp CACHE PATH "Jsoncpp prefix")

  find_package(jsoncpp REQUIRED NO_DEFAULT_PATH)

endmacro()

