// Example for Raspberry Pi RP2040

#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

static void heartbeat_task(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(500);
    }
}

int main()
{
    setup_default_uart();
    printf("Hello, world!\n");

    xTaskCreate(heartbeat_task, "heartbeat", configMINIMAL_STACK_SIZE,
        nullptr, 1, nullptr);
    vTaskStartScheduler();

    return 0;
}
