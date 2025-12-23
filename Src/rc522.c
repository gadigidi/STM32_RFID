#include "rc522.h"
#include "rfid_hw.h"
#include "timebase.h"
#include "spi.h"
#include "gpio.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

void rc522_init(void) {
    rc522_hard_reset();

}

void rc522_hard_reset(void) {
    rfid_assert_reset();
    while (!(timebase_blocking_delay_ms(25))) {
    }
    rfid_deassert_reset();
    while (!(timebase_blocking_delay_ms(5))) {
    }
}

void rc522_write_reg(uint8_t addr, uint8_t data) {
    uint8_t command = (addr & RC522_ADDR_MASK) | (RC522_WR << 7);
    rfid_assert_cs(); //CS = 0
    uint8_t dummy = spi1_txrx_byte(command);
    dummy = spi1_txrx_byte(data);
    rfid_deassert_cs(); //CS = 1
}

uint8_t rc522_read_reg(uint8_t addr) {
    uint8_t command = ((addr << 1) & 0x7e) | 0x80;
    rfid_assert_cs();
    uint8_t dummy = spi1_txrx_byte(command);
    uint8_t data = spi1_txrx_byte(0xaa);
    //rfid_deassert_cs();
    return data;
}

void rc522_clean_irq(void) {

}


