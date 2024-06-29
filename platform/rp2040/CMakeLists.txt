# Note that platform_include.cmake must be loaded before this file!
if(NOT TARGET _rp2040_platform_include_defined)
    message(FATAL_ERROR "must include ${CMAKE_CURRENT_LIST_DIR}/platform_include.cmake before adding rp2040 HAL target")
endif()

pico_sdk_init()

# The RP2040 SDK requires a custom FreeRTOS kernel. Bring it in here.
include(FreeRTOS_Kernel_import.cmake)

# Define the library that contains our board-specific code
add_library(platform_hal STATIC)
target_sources(platform_hal PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/hal.c)

# Link always-required libraries
target_link_libraries(platform_hal PUBLIC
    pico_stdlib
    FreeRTOS-Kernel-Heap4
)

target_include_directories(platform_hal PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/include
)

if(PICO_BOARD STREQUAL pico_w)
    message(STATUS "pico_w board configuration")
    add_compile_definitions(USING_CYW43=1)
    if(HW_WIFI)
        message(STATUS "pico_w wifi enabled")
        target_link_libraries(platform_hal PUBLIC
            pico_cyw43_arch_lwip_sys_freertos
        )
    else()
        message(STATUS "pico_w wifi not enabled")
        target_link_libraries(platform_hal PUBLIC
            pico_cyw43_arch_none
        )
    endif()
elseif(HW_WIFI)
    message(FATAL_ERROR "cannot enable wifi support for boards other than the pico_w")
endif()

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