
cmake_minimum_required(VERSION 3.20)

file(STRINGS 
  ${CMAKE_CURRENT_LIST_DIR}/src/spy/version.h
  SPY_VERSION_MACRO REGEX "#define SPY_VERSION_STRING \"[0-9]+.[0-9]+.[0-9]+\"$"
)
string(REGEX REPLACE
  "#define SPY_VERSION_STRING \"([0-9]+.[0-9]+.[0-9]+)\"$" "\\1"
  SPY_VERSION "${SPY_VERSION_MACRO}"
)

project(TelegramSpyBackend 
  VERSION ${SPY_VERSION} LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SPY_PACKAGED_BUILD "If ON builds all libraries from source to port in standalone folder" OFF)
option(SPY_BUILD_DOCUMENTATION "Builds documentation to 'docs/html' folder" OFF)
option(SPY_ENABLE_TESTING "If ON builds tests" ON)
option(SPY_ENABLE_OPTIMIZATION "If ON compile spy with optimization" OFF)

##################################################################################################################
## Simple projectconfiguration

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

message("======================================================")
message("TelegramSpy build type: ${CMAKE_BUILD_TYPE}")
message("TelegramSpy install into: ${SPY_INSTALLDITECTORY_NAME}")
message("======================================================")

##################################################################################################################


## Resolve dependencies 
add_subdirectory(external)

## Build binaries
add_subdirectory(src)

## Build documentation
if (${SPY_BUILD_DOCUMENTATION})
  add_subdirectory(doc)
endif()


install(DIRECTORY resources DESTINATION ${CMAKE_INSTALL_PREFIX})
