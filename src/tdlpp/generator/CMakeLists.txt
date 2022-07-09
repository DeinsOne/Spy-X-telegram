
add_executable(binding_generator src/generator.cpp)

find_package(Filesystem REQUIRED)

target_link_libraries(binding_generator PRIVATE std::filesystem)
target_compile_definitions(binding_generator
  PRIVATE -DCXX_FILESYSTEM_IS_EXPERIMENTAL=${CXX_FILESYSTEM_IS_EXPERIMENTAL}
)

## Generate bindings header here
set(TDLPP_BINDING_HEADER ${CMAKE_BINARY_DIR}/IdNameBinding.hpp)

add_custom_command(
  TARGET binding_generator POST_BUILD
  COMMAND $<TARGET_FILE:binding_generator>
    ${Td_DIR}/../../../include/td/telegram/td_api.h
    ${TDLPP_BINDING_HEADER}
  VERBATIM
)
