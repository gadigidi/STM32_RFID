#include "rc522.h"
#include "rc522_ports.h"
#include "rc522_debug.h"
#include "timebase.h"
#include "spi.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

void rc522_chip_init(void) {
    rc522_ports_init();
    rc522_hard_reset();
    rc522_soft_reset();
    rc522_read_version();
    //if (!rc522_alive_status()) {
        //Do something
    //}
    rc522_clean_flags();
    rc522_clean_fifo();
    rc522_enable_irq();
    rc522_config_timeout(); //Config RC522 timer
    rc522_config_mode();
    rc522_config_crc();
    rc522_config_txask();
    rc522_turn_on_antennas(); //Turn on antennas
}

void rc522_hard_reset(void) {
    rc522_assert_reset();
    timebase_blocking_delay_ms(5);
    rc522_deassert_reset();
    timebase_blocking_delay_ms(5);
}

void rc522_write_reg(uint8_t addr, uint8_t data) {
    uint8_t command = ((addr << 1) & RC522_ADDR_MASK) | (RC522_WR << 7);
    rc522_assert_cs(); //CS = 0
    uint8_t dummy = spi1_txrx_byte(command);
    (void) dummy;
    dummy = spi1_txrx_byte(data);
    rc522_deassert_cs(); //CS = 1
}

uint8_t rc522_read_reg(uint8_t addr) {
    uint8_t command = ((addr << 1) & RC522_ADDR_MASK) | (RC522_RD << 7);
    rc522_assert_cs();
    uint8_t dummy = spi1_txrx_byte(command);
    (void) dummy;
    uint8_t data = spi1_txrx_byte(0xFF);
    rc522_deassert_cs();
    return data;
}

void rc522_modify_reg(uint8_t addr, uint8_t new_data) {
    uint8_t old_data = rc522_read_reg(addr);
    uint8_t data = old_data | new_data;
    rc522_write_reg(addr, data);
}

void rc522_soft_reset(void) {
    rc522_write_reg(RC522_COMMAND_REG, RC522_COMMAND_RESET);
    timebase_blocking_delay_ms(25);
    rc522_write_reg(RC522_COMMAND_REG, RC522_COMMAND_IDLE);
}

static bool chip_alive = 0;
void rc522_read_version(void) {
    uint8_t version = rc522_read_reg(RC522_VERSION_REG);
    if (version != RC522_VERSION) {
        chip_alive = 0;
    } else {
        chip_alive = 1;
    }
}

bool rc522_alive_status(void) {
    return chip_alive;
}

void rc522_enable_irq(void) {
    //COMIRQREG.SET1 = 0. Then writing '1' to each IRQ field in COMIRQREG is *clearing* the field
    rc522_write_reg(RC522_COMIRQ_REG, (0 << 7));
    rc522_write_reg(RC522_COMIRQ_REG, 0x7FU); //Clear all IRQs

    uint8_t value = ((1U << 0) | (1U << 1) | (1U << 5)); //Enable IRQ for timeout, error, RX
    value |= (1U << 7); //Keep bit 7 IRQINV=1. This make IRQ pin active low
    rc522_write_reg(RC522_COMIEN_REG, value);
    //rc522_debug();
}

void rc522_clean_flags(void) {
    //Clear IRQ flags for timeout, error, RX
    //uint8_t value = ((1U << 0) | (1U << 1) | (1U << 5));
    rc522_write_reg(RC522_COMIRQ_REG, 0x7FU);
    //rc522_debug();
}

void rc522_clean_fifo(void) {
    rc522_write_reg(RC522_FIFOLEVEL_REG, (1U << 7));
}

void rc522_config_timeout(void) {

    uint8_t value = 0;

    //Prescaler
    value = RC522_PSC & 0xFFU; //8 LSB of prescaler
    rc522_write_reg(RC522_T_PRESCALER_REG, value);

    //Reload 8 MSB
    value = (RC522_RELOAD_VAL >> 8) & 0xFFU; //8 MSB of reload val
    rc522_write_reg(RC522_T_RELOAD_H_REG, value);

    //Reload 8 LSB
    value = RC522_RELOAD_VAL & 0xFFU; //8 LSB of reload val
    rc522_write_reg(RC522_T_RELOAD_L_REG, value);

    //TMODE
    value = (RC522_PSC >> 8) & 0xFU; //4 MSB of prescaler
    value |= (1U << 7); //T-Auto on
    rc522_modify_reg(RC522_T_MODE_REG, value);

    //timebase_blocking_delay_ms(1000);
    //rc522_debug();
}

void rc522_config_txask(void) {
    //Forcing TxASK = 100. Reader not recognize card without it
    rc522_write_reg(RC522_TXASK_REG, 0x40);
}

void rc522_turn_on_antennas(void) {
    rc522_modify_reg(RC522_TXCONTROL_REG, 0x3);
    //rc522_debug();
}

void rc522_config_mode(void) {
    //ModeReg

}

void rc522_config_crc(void) {

}

void rc522_pre_transcieve(void) {
    rc522_write_reg(RC522_COMMAND_REG, RC522_COMMAND_IDLE); //Command = IDLE
    rc522_clean_flags();
    rc522_clean_fifo();
}

void rc522_transceive_start(void) {

    //Command = transcieve
    rc522_write_reg(RC522_COMMAND_REG, RC522_COMMAND_TRANSCIEVE);
    //rc522_debug();

    //BITFRAMING.START_SEND
    uint8_t value = (1U << 7);
    rc522_modify_reg(RC522_BITFRAMING_REG, value);

    rc522_debug();
    //Back to IDLE
    //rc522_write_reg(RC522_COMMAND_REG, RC522_COMMAND_IDLE);
}

uint8_t rc522_read_fifo(uint8_t *fifo) {
    uint8_t length = rc522_read_reg(RC522_FIFOLEVEL_REG);

    for (int i = 0; i < length; i++) {
        fifo[i] = rc522_read_reg(RC522_FIFODATA_REG);
    }

    return length;
}

void rc522_send_reqa(void) {
    rc522_pre_transcieve();
    rc522_modify_reg(RC522_BITFRAMING_REG, 7); //Last byte of tx is 7 bits
    uint8_t reqa = 0x26 & 0x7FU;
    rc522_write_reg(RC522_FIFODATA_REG, reqa);
    rc522_transceive_start();
    //rc522_write_reg(RC522_BITFRAMING_REG, 0); //Last byte of tx is 8 bits
}

void rc522_get_atqa(uint8_t *fifo) {
    uint8_t length = rc522_read_fifo(fifo);
    if (length != 2) {

    }
}

void rc522_read_uid(void) {

}











