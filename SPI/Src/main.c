#include "spi.h"
#include "seg7.h"
#include "timebase.h"
#include "gpio.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

int main(void){

    timebase_init();
    
    spi1_init();
    spi1_config_baudrate(SPI_BR_DIV64);
    spi1_enable_clock();
    spi1_config_mode(SPI_MASTER);
    spi1_config_clock_mode(SPI_NSS_SOFTWARE);
    spi1_config_bit_order(SPI_MSB_FIRST);
    spi1_config_nss(SPI_NSS_SOFTWARE);
    spi1_config_data_size(SPI_DATASIZE_8BIT);
    spi1_enable(1U);

    spi1_send_byte(0xa);
    //uint8_t rx = spi1_read_dr();
    volatile int j = 0;
    while (1) {
        j++;
    }
}
