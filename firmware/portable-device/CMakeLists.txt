add_library(portable-device INTERFACE)
target_include_directories(portable-device INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(portable-device INTERFACE pico_stdlib hardware_gpio hardware_spi hardware_pwm)
target_sources(portable-device INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/portable_device.c
    ${CMAKE_CURRENT_LIST_DIR}/portable_device.h)
