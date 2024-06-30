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
    cli_uart_puts("WiFi service started\r\n");

    typedef enum {
      STATE_CONFIGURING,
      STATE_CONFIG_FAILED,
      STATE_CONFIG_SUCCESS,
      STATE_CONNECTING,
      STATE_CONNECTED,
      STATE_FAULT,
      STATE_COMPLETE
    } t_state;

    t_state state = STATE_CONFIGURING;
    char *msg = pvPortMalloc(256);

    while (state != STATE_FAULT) {
        switch (state) {
            case STATE_CONFIGURING:cli_uart_puts(timestamp());
                cli_uart_puts("Configuring CYW43 module\r\n");
                int err = cyw43_arch_init();
                if (err) {
                    state = STATE_CONFIG_FAILED;
                    break;
                }

                cli_uart_puts(timestamp());
                cli_uart_puts("CYW43 module initialized.\r\n");
                state = STATE_CONFIG_SUCCESS;
                break;

            case STATE_CONFIG_FAILED:cli_uart_puts(timestamp());
                cli_uart_puts("CYW43 module could not initialize. WiFi service will terminate.\r\n");
                state = STATE_FAULT;
                break;

            case STATE_CONFIG_SUCCESS:cli_uart_puts(timestamp());
                sprintf(msg, "Connecting to '%s'\r\n", WIFI_SSID);
                cli_uart_puts(msg);
                cyw43_arch_enable_sta_mode();
                cli_uart_puts(timestamp());
                cli_uart_puts("Station mode enabled.\r\n");
                if (cyw43_arch_wifi_connect_blocking(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_MIXED_PSK)) {
                    cli_uart_puts(timestamp());
                    cli_uart_puts("Connection request faulted.\r\n");
                    state = STATE_FAULT;
                } else {
                    cli_uart_puts(timestamp());
                    cli_uart_puts("Connection request made.\r\n");
                    state = STATE_CONNECTING;
                }
                break;

            case STATE_CONNECTING:cli_uart_puts(timestamp());
                int link_state = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
                cli_uart_puts(timestamp());
                switch (link_state) {
                    case CYW43_LINK_UP:
                        cli_uart_puts("Connected.\r\n");
                        state = STATE_CONNECTED;
                        break;

                    case CYW43_LINK_DOWN:
                        cli_uart_puts("Disconnected from WiFi. Attempting to reconnect.\r\n");
                        cyw43_arch_wifi_connect_async(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_MIXED_PSK);
                        break;

                    case CYW43_LINK_JOIN:
                        cli_uart_puts("Connected to WiFi. Obtaining IP address.\r\n");
                        break;

                    case CYW43_LINK_FAIL:
                        cli_uart_puts("Connection failed: No details.\r\n");
                        state = STATE_FAULT;
                        break;

                    case CYW43_LINK_NONET:
                        cli_uart_puts("Connection failed: Could not find SSID.\r\n");
                        state = STATE_FAULT;
                        break;

                    case CYW43_LINK_BADAUTH:
                        cli_uart_puts("Connection failed: Bad wifi credentials.\r\n");
                        state = STATE_FAULT;
                        break;

                    default:
                        cli_uart_puts("Unexpected value from cyw43_wifi_link_status. Aborting wifi service.\r\n");
                        state = STATE_FAULT;
                }
                cli_uart_puts(msg);
                break;

            case STATE_CONNECTED:
                sprintf(msg, "IP Address %s\r\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
                cli_uart_puts(timestamp());
                cli_uart_puts(msg);
                state = STATE_COMPLETE;
                break;

            default:
                cli_uart_puts(timestamp());
                cli_uart_puts("Unexpected WiFi state. WiFi service terminating.\r\n");
                state = STATE_FAULT;
        }

        task_delay_ms(1000);
    }

    vPortFree(msg);
    vTaskDelete(NULL);
}