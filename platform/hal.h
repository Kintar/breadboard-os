#ifndef HAL_INCLUDE_H
#define HAL_INCLUDE_H

#include <stdint.h>
#include <stdbool.h>

/***********************************************************************
 * General utility items for the hardware abstraction layer
 *
 * These items are implemented by the core HAL, and do not need to be
 * implemented in the board-specific HAL.
 */

// Possible error codes returned by HAL functions.
typedef enum {
  Err_Success,
  Err_GeneralError,
  Err_InvalidParameter,
  Err_OutOfMemory,
  Err_Timeout,
} hal_err_code_t;

/**
 * @brief Convert a hal_err_code_t to a printable string
 * @param err
 * @return a null-terminated string
 */
const char *hal_errToA(hal_err_code_t err);

/**
 * @brief Retrieve details about the last error encountered by the HAL
 *
 * @param msg_buffer pointer to a space in memory to hold the null-terminated result
 * @param buf_len length of the provided buffer
 *
 * @return true if a message was available and copied to the buffer
 */
bool hal_get_last_err_msg(char *msg_buffer, uint8_t buf_len);


/***********************************************************************
 * Items from here down are required to be implemented by the board
 * specific HAL, if supported by the board. Board-specific HALs SHOULD
 * throw a compiler error if an unsupported feature is enabled, but
 * MAY choose to turn the access into a no-op.
 */

/**
 * @brief Performs any board-specific setup required before hardware is accessed
 */
void hal_init();

#if HW_ONBOARD_LED

void hal_onboard_led_set(bool state);

bool hal_onboard_led_get();

#endif

#if HW_GPIO

// todo: it's probably a good idea to allow the board-hal library to define the type of gpio values
void hal_gpio_put(uint8_t pin, uint32_t value);

uint32_t hal_gpio_get(uint8_t pin);

#endif

#if HW_I2C
#endif

#if HW_WIFI
typedef enum {
  None,
  WPA,
  WPA2,
  WPA2_PSK_TKIP,
  WPA2_PSK_AES,
  WPA2_PSK_MIXED
} hal_wifi_auth_t;

hal_err_code_t hal_wifi_init();

void hal_wifi_deinit();

hal_err_code_t hal_wifi_init_with_country(uint32_t country);

void hal_wifi_set_sta_mode();

void hal_wifi_set_ap_mode();

hal_err_code_t hal_wifi_connect(const char *ssid, const char *password, hal_wifi_auth_t auth_mode);

#endif

#if HW_UART

hal_err_code_t hal_uart_init();

void hal_uart_deinit();

#endif


#endif /* HAL_INCLUDE_H */
