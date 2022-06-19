
add_executable(binding_generator src/generator.cpp)
target_link_libraries(binding_generator
  PRIVATE stdc++fs
)

if (${SPY_ENABLE_OPTIMIZATION})
  target_compile_options(binding_generator PRIVATE -O2)
endif()

## Generate bindings header here
set(TDLPP_BINDING_HEADER ${CMAKE_BINARY_DIR}/IdNameBinding.hpp)

add_custom_command(
  TARGET binding_generator POST_BUILD
  COMMAND $<TARGET_FILE:binding_generator>
    ${Td_DIR}/../../../include/td/telegram/td_api.h
    ${TDLPP_BINDING_HEADER}
  VERBATIM
)
