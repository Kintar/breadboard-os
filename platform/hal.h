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
  HAL_ERR_SUCCESS,
  HAL_ERR_GENERAL_ERROR,
  HAL_ERR_INVALID_PARAMETER,
  HAL_ERR_OUT_OF_MEMORY,
  HAL_ERR_TIMEOUT,
  HAL_ERR_UNSUPPORTED,
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

/***********************************************************************
 * Interfaces to the onboard LED
 */
#if HW_ONBOARD_LED

void hal_onboard_led_set(bool state);

bool hal_onboard_led_get();

#endif

/***********************************************************************
 * Interfaces to the board's GPIO pins
 */
#if HW_GPIO

// todo: it's probably a good idea to allow the board-hal library to define the type of gpio values
void hal_gpio_put(uint8_t pin, uint32_t value);

uint32_t hal_gpio_get(uint8_t pin);

#endif

/***********************************************************************
 * Interfaces to I2C communication.
 *
 * Note: if the board does not have i2c hardware support, the HAL implementer
 * is free to provide a bitbang library for it, but should NOT present it
 * as hardware-supported. Using a timing-critical library in FreeRTOS
 * requires careful consideration of task priorities and preemption.
 *
 * TODO: Need a way to identify boards without dedicated i2c hardware?
 */
#if HW_I2C
#endif

/***********************************************************************
 * Interfaces to the board's WiFi subsystem.
 */
#if HW_WIFI
/**
 * Standard WiFi authentication types. A board whose WiFi stack does not implement
 * a given auth scheme should return HAL_ERR_UNSUPPORTED.
 */
typedef enum {
  HW_WIFI_AUTH_NONE,
  HW_WIFI_AUTH_WPA,
  HW_WIFI_AUTH_WPA2,
  HW_WIFI_AUTH_WPA2_PSK_TKIP,
  HW_WIFI_AUTH_WPA2_PSK_AES,
  HW_WIFI_AUTH_WPA2_PSK_MIXED
} hal_wifi_auth_t;

hal_err_code_t hal_wifi_init();

void hal_wifi_deinit();

hal_err_code_t hal_wifi_init_with_country(uint32_t country);

void hal_wifi_set_sta_mode();

void hal_wifi_set_ap_mode();

hal_err_code_t hal_wifi_connect(const char *ssid, const char *password, hal_wifi_auth_t auth_mode);

#endif

/***********************************************************************
 * Interfaces to the board's dedicated UART.
 */
#if HW_UART

hal_err_code_t hal_uart_init();

void hal_uart_deinit();

#endif


#endif /* HAL_INCLUDE_H */
