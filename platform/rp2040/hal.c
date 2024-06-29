
/**
 * @file
 * Provides rp-2040-specific implementations of the bbos hal.h headers.
 *
 * @author Alec Lanter
 * @date 2024-06-29
 */

#include "hal.h"
#include "pico/stdio.h"
#include "hardware/timer.h"

#ifdef HW_ONBOARD_LED
#include "pico/printf.h"
void hal_onboard_led_set(bool state) {
    printf("LED set to %b\n", state);
}

bool hal_onboard_led_get() {
    printf("LED is...uhm...fake?\n");
}
#endif

void hal_init() {
    stdio_init_all();
}

uint64_t get_time_us(void){
    return time_us_64();
}
