#include "joystick.h"
#include "adc.h"
#include "gpio.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"


void joystick_init (void){

    gpio_init(GPIOAEN);

    adc1_pa0_pa1_init();
    //Set sample time for PA0 (x axis)
    adc1_set_SMPR (0, 7U);
    //Set sample time for PA1 (y axis)
    adc1_set_SMPR (1, 7U);
    adc1_set_sequence_length(0);
    adc1_set_adon();
}

uint16_t joystick_read_axis_val (char axis){
    uint16_t val = 0;
    
    if (axis == 'x'){
      adc1_set_SQR (0, 0);
    }
    else {
      adc1_set_SQR (0, 1);
    }

    adc1_start_conversion();
    val = adc1_read();
    printf("conversion done. val = %d", val);
    
    return val;
}

