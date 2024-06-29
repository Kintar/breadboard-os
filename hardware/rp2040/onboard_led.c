/******************************************************************************
 * @file onboard_led.c
 *
 * @brief Functions to interact with the onboard LED GPIO. The implementation of
 *        these functions is MCU-specific and will need to be changed if ported
 *        to a new hardware family.
 *
 * @author Cavin McKinley (MCKNLY LLC)
 *
 * @date 02-14-2024
 * 
 * @copyright Copyright (c) 2024 Cavin McKinley (MCKNLY LLC)
 *            Released under the MIT License
 * 
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "hardware_config.h"
#include "pico/stdlib.h"


#ifdef USING_CYW43
#include "pico/cyw43_arch.h"
static bool cyw43_enabled = false;

/**
 * DO NOT USE THIS FUNCTION unless you're updating the wifi service module. I'm
 * still trying to think of a better way to structure access to this variable. :P
 * -- Kintar
 */
void __set_cyw43_enabled(bool status) {
    cyw43_enabled = status;
}

void onboard_led_init(void) {
#if !HW_WIFI
    if(cyw43_arch_init()) {
        uart_puts(UART_ID_CLI, timestamp());
        uart_puts(UART_ID_CLI, "Failed to initialize CYW43 hardware.\r\n");
    } else {
        uart_puts(UART_ID_CLI, timestamp());
        uart_puts(UART_ID_CLI, "Initialized onboard CYW43 hardware\r\n");
        cyw43_enabled = true;
    }
#else
    cli_uart_puts(timestamp());
    cli_uart_puts("Skipping onboard led init in deference for WiFi service.\r\n");
#endif
}

void onboard_led_set(bool led_state) {
    if (!cyw43_enabled) {
        cli_uart_puts("cyw43 is not enabled, cannot control onboard LED yet");
    } else {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
    }
}

bool onboard_led_get(void) {
    return cyw43_enabled && cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
}
#else
void onboard_led_init(void) {
    gpio_init(PIN_NO_ONBOARD_LED);
    gpio_set_dir(PIN_NO_ONBOARD_LED, GPIO_OUT);
}

void onboard_led_set(bool led_state) {
    gpio_put(PIN_NO_ONBOARD_LED, led_state);
}

bool onboard_led_get(void) {
    return gpio_get(PIN_NO_ONBOARD_LED);
}
#endif
