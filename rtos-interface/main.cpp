#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

int arg1[2] = {25, 1};

void sendBinGPIO(void *param)
{	
	int *argument;
	argument = (int *) param;
	int pin = argument[0];
	int onoff = argument[1];
   try {
   	gpio_put(pin, onoff);
	throw (pin);
   }
   catch(unsigned int pin) {
   	gpio_init(pin);
	gpio_set_dir(pin, GPIO_OUT);
   }
}

int main() 
{
    stdio_init_all();
    
    gpio_init(0);
    gpio_set_dir(0, GPIO_IN);
    gpio_pull_up(0);

    TaskHandle_t sendBinGPIOtask = NULL;

    uint32_t status = xTaskCreate(
                    sendBinGPIO,
                    "Send Binary GPIO",
                    1024,
                    &arg1,
                    tskIDLE_PRIORITY,
                    &sendBinGPIOtask);

    for( ;; )
    {
        if (!gpio_get(0)) {
		arg1[1] = 1;
		vTaskSuspend(sendBinGPIOtask);
		vTaskResume(sendBinGPIOtask);
	} else {
		arg1[1] = 0;
		vTaskSuspend(sendBinGPIOtask);
		vTaskResume(sendBinGPIOtask);
	}
    }

}
