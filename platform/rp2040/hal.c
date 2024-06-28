#include "hal.h"
#include "pico/stdio.h"

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