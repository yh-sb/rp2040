// Example for Raspberry Pi RP2040

#include <stdio.h>
#include "periph/gpio.hpp"
#include "FreeRTOS.h"
#include "task.h"

static void heartbeat_task(void *pvParameters)
{
    periph::gpio led(25, periph::gpio::mode::DO);
    
    while(1)
    {
        led.toggle();
        vTaskDelay(500);
    }
}

int main()
{
    xTaskCreate(heartbeat_task, "heartbeat", configMINIMAL_STACK_SIZE,
        nullptr, 1, nullptr);
    vTaskStartScheduler();

    return 0;
}
