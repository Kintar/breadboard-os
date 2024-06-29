#include "hardware_config.h"
#include "services.h"
#include "rtos_utils.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pico/cyw43_arch.h"

static void prvWifiTask(void *pvParams);

TaskHandle_t xWifiTask;

BaseType_t wifi_service(void) {

    BaseType_t xReturn;

    // create the FreeRTOS task
    xReturn = xTaskCreate(
            prvWifiTask,
            xstr(SERVICE_NAME_WIFI),
            STACK_WIFI,
            NULL,
            PRIORITY_WIFI,
            &xWifiTask
    );

    cli_uart_puts(timestamp());
    if (xReturn == pdPASS) {
        cli_uart_puts("WiFi service started\r\n");
    } else {
        cli_uart_puts("Error starting wifi service\r\n");
    }

    return xReturn;
}

static void prvWifiTask(void *pvParams) {
    cli_uart_puts(timestamp());
    cli_uart_puts("initializing wifi module...");

    // NOTE: This should be a config option somewhere. Country init makes wifi more responsive
    // in some situations.
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_USA)) {
        cli_uart_puts("failed. WiFi service terminating.\r\n");
        vTaskDelete(NULL);
        return;
    }

    cli_uart_puts("success.\r\n");
    char *msg = pvPortMalloc(50 + strlen(WIFI_SSID));
    sprintf(msg, "%sAttempting connection to SSID '%s'.\r\n", (const char *) timestamp(), WIFI_SSID);
    cli_uart_puts(msg);
    bool waiting_for_link = true;
    int err = cyw43_arch_wifi_connect_async(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_MIXED_PSK);
    if (err) {
        waiting_for_link = false;

        cli_uart_puts(timestamp());
        sprintf(msg, "WiFi scan for connection failed: %d", err);
        cli_uart_puts(msg);
    }

    vPortFree(msg);

    // TODO : Long-term, this should be a state machine for monitoring wifi activity based on current state
    while (waiting_for_link) {
        switch (cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA)) {
            case CYW43_LINK_UP:cli_uart_puts(timestamp());
                cli_uart_puts("WiFi connected.\r\n");
                waiting_for_link = false;
            default:cli_uart_puts(timestamp());
                cli_uart_puts("WiFi not connected.\r\n");
        }

        task_sched_update(REPEAT_WIFI, DELAY_WIFI);
    }

    vTaskDelete(NULL);
}