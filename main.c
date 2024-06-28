#include <stdio.h>
#include <pico/cyw43_arch.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hal.h"

static void main_task(void *taskParameters) {
    TickType_t delay_ticks = (TickType_t)(2500 * configTICK_RATE_HZ / 1000); // convert millisecond delay to OS ticks

    while (true) {
        puts("ba-bump...");

        vTaskDelay(delay_ticks);
    }
}

static void wifi_task(void *taskArgs) {
    puts("initializing onboard wifi system...");
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_USA)) {
        puts("failed!\n");
    } else {
        puts("done.\n");
    }

    puts("Entering station mode...\n");
    cyw43_arch_enable_sta_mode();

    puts("Attempting to join wifi network...\n");
    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        exit(1);
    } else {
        printf("connected.\n");
        printf("IP address is %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
    }

    cyw43_arch_deinit();

    vTaskDelete(NULL);
}

int main() {
    hal_init();
    puts("initializing...\n");
    sleep_ms(1000);

    xTaskCreate(main_task,
                "main",
                configMINIMAL_STACK_SIZE,
                NULL,
                1,
                NULL);

    xTaskCreate(wifi_task,
                "wifi",
                configMINIMAL_STACK_SIZE,
                NULL,
                1,
                NULL);

    vTaskStartScheduler();

    puts("Oh, no....\n\n\n");
}