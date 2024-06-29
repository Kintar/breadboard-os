# ./platform
## Base Hardware Abstraction Layer

This directory contains `hal.h` and `hal.c`, the two core files responsible for abstraction
board-specific code away from the main system.

Every board-specific library must implement the functions defined in `hal.h`, since the system
itself uses these interfaces to interact with the board hardware.