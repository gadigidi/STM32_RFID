#include "seg7.h"
#include "gpio.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"


int main(void){

    //gpio_init(GPIOAEN);

    seg7_init();
    seg7_deselect_digits();
    seg7_select_seg(SEG_A_PIN);
    seg7_select_digit(DIG1_PIN);

    while(1){


    	//printf("conversion done. y_val = %d", y_val);

    }
}
