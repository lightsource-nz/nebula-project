cmake_minimum_required(VERSION 3.18)

get_filename_component(LIGHT_PATH "${LIGHT_PATH}" REALPATH BASE_DIR "${CMAKE_CURRENT_LIST_DIR}")

include(${LIGHT_PATH}/external/light_preinit.cmake)

project(nebula-project C CXX ASM)

add_subdirectory(${LIGHT_PATH}/ light_framework)

# modules
add_subdirectory(module/light_pulse)
add_subdirectory(module/nebula)
add_subdirectory(module/nebula_pulse)
