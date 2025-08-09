#include <stdio.h>
#include "stm32f4xx.h"
#include "fpu.h"
#include "uart.h"

/*Modules:
 * FPU
 * UART
 * GPIO
 * TIMEBASE
 * */



#define  GPIOAEN		(1U<<0)
#define  PIN5			(1U<<5)
#define  LED_PIN		PIN5


int main()
{
	/*Enable FPU*/
	fpu_enable();

	/*Initialize debug UART*/
	debug_uart_init();

	while(1)
	{

		printf("Hello from STM32\n\r");
		for(int i = 0; i < 9000; i++){}
	}
}

