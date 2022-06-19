
cmake_minimum_required(VERSION 3.20)

find_package(Git REQUIRED)
include(ProcessorCount)


## Function to install single module with specific name
function(install_module_globally NAME)

  ## Clone module
  execute_process(
    COMMAND 
      ${GIT_EXECUTABLE} clone --depth=1 "https://github.com/oatpp/${NAME}" tmp/${NAME}
  )

  ## Configure module
  execute_process(
    COMMAND
      ${CMAKE_COMMAND} -S tmp/${NAME} -B tmp/build/${NAME} -DCMAKE_BUILD_TYPE=Release -DOATPP_BUILD_TESTS=OFF -DOATPP_SQLITE_AMALGAMATION=ON
  )


  ProcessorCount(PROCESSOR_COUNT)

  ## Build module
  execute_process(
    COMMAND
      ${CMAKE_COMMAND} --build tmp/build/${NAME} --config release -j ${PROCESSOR_COUNT}
  )

  ## Install module
  execute_process(
    COMMAND
      ${CMAKE_COMMAND} --install tmp/build/${NAME}
  )

endfunction()


## Define list of modules to be installed
set(OATPP_MODULES oatpp oatpp-swagger oatpp-sqlite)


## Make temp directory 
file(MAKE_DIRECTORY tmp tmp/build)

foreach(MODULE_NAME IN LISTS OATPP_MODULES)
  message(STATUS "Install module ${MODULE_NAME}")

  install_module_globally(${MODULE_NAME})
endforeach()


file(REMOVE_RECURSE tmp)
