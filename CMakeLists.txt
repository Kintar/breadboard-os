###################################
# Standard cmake boilerplate stuff
cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

######################################################################
# Per-project customization is handled in project.cmake
include(project.cmake)
message(STATUS "configured for platform ${PLATFORM} and board ${BOARD}")

set(PLATFORM_DIR ${CMAKE_CURRENT_LIST_DIR}/platform/${PLATFORM})
message(STATUS "platform directory is ${PLATFORM_DIR}")

######################################################################
# The presence of a platform_include.cmake in the platform directory
# for a board is required to build the HAL
if(EXISTS ${PLATFORM_DIR}/platform_include.cmake)
    include(${PLATFORM_DIR}/platform_include.cmake)
endif()

# todo : this will be made optional once the wifi CLI command is in place
######################################################################
# Allow the wifi_ssid and wifi_password to be built into the firmware
add_compile_definitions(WIFI_SSID=\"$ENV{WIFI_SSID}\" WIFI_PASSWORD=\"$ENV{WIFI_PASSWORD}\")

######################################################################
# Finally getting to the meat of configuration.
project(${PROJ_NAME}
    VERSION ${PROJ_VER}
    LANGUAGES C CXX ASM
)

add_executable(${CMAKE_PROJECT_NAME} main.c)

######################################################################
# The root hardware abstraction layer is built here.
# By default, GPIO, I2C, flash memory, and any hardware components
# required by the platform's FreeRTOS port are included in the HAL.
# The various parts of the HAL can be toggled through configuration
# options in platform/CMakeLists.txt, although FreeRTOS and its
# dependencies are always required.
add_subdirectory(platform)

######################################################################
# this brings in the board-specific HAL imlpementations
add_subdirectory(${PLATFORM_DIR})

#####################################################################
# By the time we get here we should have the FreeRTOS kernel library
# defined and included. This should have been done by the platform
# library, and if not you'll need to bring it in yourself.
if(NOT TARGET _FreeRTOS_kernel_inclusion_marker)
    message(FATAL_ERROR
        "FreeRTOS-Kernel library not found. Ensure the platform library is properly linking it"
    )
endif()

######################################################################
# link FreeRTOS and the HAL into the firmware
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    FreeRTOS-Kernel
    hal
    platform_hal
)

######################################################################
# Edit system/system_include.cmake to enable/disable the various sys
# modules.
add_subdirectory(system)

######################################################################
# The pico SDK requires that certain SDK commands be called on the
# primary output, which we can only do from the same directory as the
# list file that created that output. This hook is defined to satisfy
# that condition.
if(COMMAND platform_extras)
    platform_extras()
endif()