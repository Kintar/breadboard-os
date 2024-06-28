set(PICO_BOARD ${BOARD})
set(PICO_PLATFORM ${PLATFORM})

# Pull in Raspberry Pi Pico SDK (must be before project)
include(${PLATFORM_DIR}/pico_sdk_import.cmake)

if(PICO_SDK_VERSION_STRING VERSION_LESS "1.5.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()
