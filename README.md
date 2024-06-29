# Purpose

This branch is a complete rebuild of the BreadBoard-OS project to restructure the build process.
It came about from my attempt to integrate `pico_cyw43_arch_lwip_sys_freertos` into mcknly's original
BBOS project https://github.com/mcknly/breadboard-os, which was constantly frustrated by strange
compilation and runtime errors, even with code that ran perfectly fine on the same pico board when
built using only the pico-examples project's `FreeRTOSConfig.h` and `lwipopts.h` files.

It's my hope that this rebuild will provide a working lwIP stack, and a more modular separation of
the hardware and system portions of the project that will be easier to customize to whatever ends
the maker community finds for it.

# Configuration

- Project name, platform, and board are configured in project.cmake.
- Board hardware features are toggled in [./platform/CMakeLists.txt](./platform/CMakeLists.txt)
- System features are toggled in [./system/CMakeLists.txt](./system/CMakeLists.txt)

See the comments in [./CMakeLists.txt](./CMakeLists.txt) for detailed instructions.

