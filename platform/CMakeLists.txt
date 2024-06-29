add_library(hal STATIC hal.c)

##############################################################
# The following set statements enable or disable various
# pieces of hardware. If enabled, the board-specific HAL
# MUST implement the functions guarded by the associated
# #if/#else block in `hal.h`.
set(HW_GPIO 1)

set(HW_I2C 1)

set(HW_WIFI 1)

set(HW_UART 1)

set(HW_ONBOARD_LED 1)


##############################################################
# Convert all of the CMake variables to compiler definitions

target_compile_definitions(hal INTERFACE
    HW_GPIO=${HW_GPIO}
    HW_I2C=${HW_I2C}
    HW_WIFI=${HW_WIFI}
    HW_UART=${HW_UART}
    HW_ONBOARD_LED=${HW_ONBOARD_LED}
)