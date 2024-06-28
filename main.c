#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hal.h"

static void main_task(void *taskParameters) {
    TickType_t delay_ticks = (TickType_t)(500 * configTICK_RATE_HZ / 1000); // convert millisecond delay to OS ticks

    while (true) {
        puts("ba-bump...");

        vTaskDelay(delay_ticks);
    }
}

int main() {
    hal_init();
    puts("initializing...\n");
    sleep_ms(1000);
//
//    xTaskCreate(main_task,
//                "main",
//                configMINIMAL_STACK_SIZE,
//                NULL,
//                1,
//                NULL);
//
//    vTaskStartScheduler();

    puts("Oh, no....\n\n\n");
}