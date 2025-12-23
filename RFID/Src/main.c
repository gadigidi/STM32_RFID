#include "rfid.h"
#include "rc522.h"
#include "spi.h"
#include "seg7.h"
#include "timebase.h"
#include "gpio.h"

#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

int main(void){

    timebase_init();
    
    //seg7_init();
    //seg7_enable_scroll(1);


    rfid_init();
    gpio_pin_pupdr(GPIOA, 6);
    volatile uint32_t avg = 0;
    uint8_t rd_data = rc522_read_reg(0x37);
    rd_data = rc522_read_reg(0x37);
    rd_data = rc522_read_reg(0x37);

    avg = 0;
    while (1) {
        //rfid_fsm();
        //seg7_scroll_digits();
    }
}
