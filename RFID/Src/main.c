#include "rfid.h"
#include "spi.h"
#include "seg7.h"
#include "timebase.h"
#include "gpio.h"

#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

int main(void){

    timebase_init();
    
    seg7_init();
    seg7_enable_scroll(1);

    spi_init();

    rfid_init();

    while (1) {
        rfid_fsm();
        seg7_scroll_digits();
    }
}
