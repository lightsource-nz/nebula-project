add_executable(nebula_pulse)

light_framework_application_module(nebula_pulse)

target_sources(
    nebula_pulse
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/app.c
)

target_link_libraries(
    nebula_pulse
    PUBLIC
    nebula
)
