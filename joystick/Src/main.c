#include "joystick.h"
#include "gpio.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"


int main(void){

    //gpio_init(GPIOAEN);

    joystick_init();

    while(1){

    	uint16_t x_val = joystick_read_axis_val('x');
    	printf("conversion done. x_val = %d", x_val);

    	uint16_t y_val = joystick_read_axis_val('y');
    	printf("conversion done. y_val = %d", y_val);

    }
}
