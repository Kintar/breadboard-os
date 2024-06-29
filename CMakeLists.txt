cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(project.cmake)
message(STATUS "configured for platform ${PLATFORM} and board ${BOARD}")

set(PLATFORM_DIR ${CMAKE_CURRENT_LIST_DIR}/platform/${PLATFORM})
message(STATUS "platform directory is ${PLATFORM_DIR}")

if(EXISTS ${PLATFORM_DIR}/platform_include.cmake)
    include(${PLATFORM_DIR}/platform_include.cmake)
endif()

project(${PROJ_NAME}
    VERSION ${PROJ_VER}
    LANGUAGES C CXX ASM
)

add_compile_definitions(WIFI_SSID=\"$ENV{WIFI_SSID}\" WIFI_PASSWORD=\"$ENV{WIFI_PASSWORD}\")

add_executable(${CMAKE_PROJECT_NAME} main.c)

add_subdirectory(platform)

add_subdirectory(${PLATFORM_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    FreeRTOS-Kernel
    hal
    platform_hal
)


# The pico SDK requires that certain SDK commands be called on the primary output, which we
# can only do from the same directory as the list file that created that output. This hook
# is defined to satisfy that condition.
if(COMMAND platform_extras)
    platform_extras()
endif()