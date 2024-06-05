add_library(nebula INTERFACE)

target_include_directories(
    nebula
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
    nebula
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/app.c
    ${CMAKE_CURRENT_LIST_DIR}/src/nebula.c
)

target_link_libraries(
    nebula
    INTERFACE
    pulse
    light_platform
)
