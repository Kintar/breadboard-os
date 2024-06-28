pico_sdk_init()

# The RP2040 SDK requires a custom FreeRTOS kernel. Bring it in here.
include(FreeRTOS_Kernel_import.cmake)

add_library(platform_hal STATIC)

target_sources(platform_hal PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/hal.c)

target_link_libraries(platform_hal PUBLIC
    pico_stdlib
    pico_cyw43_arch_lwip_sys_freertos
    FreeRTOS-Kernel-Heap4
)

target_include_directories(platform_hal PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/include
)

## todo : include hardware libraries defined in the project file
#target_link_libraries(platform_hal
#    hardware_i2c
#    hardware_dma
#    hardware_pio
#    hardware_watchdog
#)

# function exists for SDKs which, like the Pico C/C++ SDK, require access to the firmware target
function(platform_extras)
    pico_set_program_name(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME})
    pico_set_program_version(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION})

    pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 1)
    pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 0)

    pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
endfunction()