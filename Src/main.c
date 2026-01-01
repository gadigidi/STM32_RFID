#include "rfid.h"
#include "rc522.h"
#include "rc522_debug.h"
#include "spi.h"
#include "seg7.h"
#include "timebase.h"
#include "gpio.h"

#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

int main(void){

    uint32_t time_now;
    uint8_t value;
    timebase_init();
    
    //seg7_init();
    //seg7_enable_scroll(1);

    rfid_init();

    rc522_debug();

    volatile uint32_t dummy = 0;

    dummy = 0;
    while (1) {
        time_now = timebase_show_ms();
        rfid_fsm();
        //seg7_scroll_digits();
    }
}
