#ifndef HAL_INCLUDE_H
#define HAL_INCLUDE_H

#include <stdint.h>
#include <stdbool.h>

// General utility items for the hardware abstraction layer
// These items are implemented by the project, regardless of the board.

typedef enum {
    Success,
    GeneralError,
    InvalidParameter,
    OutOfMemory,
    Timeout,
} t_hal_err_code;

typedef struct t_hal_error {
    t_hal_err_code err_code;
} t_hal_error;

const char *hal_errToA(t_hal_err_code err);

/**
 * @brief Performs any board-specific setup required by the HAL
 */
void hal_init();

// Definitions of functions the board-specific library MUST implement in order for the system to access hardware
#ifdef HW_ONBOARD_LED
void hal_onboard_led_set(bool state);
bool hal_onboard_led_get();
#endif


#endif /* HAL_INCLUDE_H */