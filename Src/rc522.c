#include "rc522.h"
#include "rc522_ports.h"
#include "rc522_debug.h"
#include "timebase.h"
#include "spi.h"
#include "exti.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

bool rc522_chip_init(void);
void rc522_hard_reset(void);

void rc522_write_reg(uint8_t addr, uint8_t data);
uint8_t rc522_read_reg(uint8_t addr);
void rc522_modify_reg(uint8_t addr, uint8_t new_data, uint8_t mask);

void rc522_soft_reset(void);
void rc522_read_version(void);
bool rc522_alive_status(void);
void rc522_enable_irq(void);
void rc522_clean_flags(void);
void rc522_clean_fifo(void);
void rc522_config_timeout(void);
void rc522_config_txask(void);
void rc522_turn_on_antennas(void);
void rc522_config_mode(void);
void rc522_config_crc(void);

bool rc522_chip_init(void) {
    rc522_ports_init();
    rc522_hard_reset();
    rc522_soft_reset();
    rc522_read_version();
    rc522_clean_flags();
    rc522_clean_fifo();
    rc522_enable_irq();
    rc522_config_timeout(); //Config RC522 timer
    rc522_config_mode();
    rc522_config_crc();
    rc522_config_txask();
    rc522_turn_on_antennas(); //Turn on antennas

    return !rc522_alive_status(); //Return 0 if chip is alive, 1 if not
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

void rc522_modify_reg(uint8_t addr, uint8_t new_data, uint8_t mask) {
    uint8_t old_data = rc522_read_reg(addr);
    uint8_t data = old_data & ~mask;
    data |= new_data;
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
    //Enable IRQ from RC522 on EXTI line
    exti_init();
    exti_enable_irq(RC522_IRQ_PIN, RC522_IRQ_PORT);

    //COMIRQREG.SET1 = 0. Then writing '1' to each IRQ field in COMIRQREG is *clearing* the field
    rc522_write_reg(RC522_COMIRQ_REG, (0 << 7));
    rc522_write_reg(RC522_COMIRQ_REG, 0x7FU); //Clear all IRQs

    uint8_t value = ((1U << 0) | (1U << 1) | (1U << 5)); //Enable IRQ for timeout, error, RX
    value |= (1U << 7); //Keep bit 7 IRQINV=1. This make IRQ pin active low
    rc522_write_reg(RC522_COMIEN_REG, value);

    rc522_write_reg(RC522_DIVIRQ_REG, (0 << 7));
    //rc522_write_reg(RC522_DIVIRQ_REG, 0x7FU); //Clear all IRQs
    rc522_write_reg(RC522_DIVIEN_REG, (1U << 2)); //Enable IRQ from CRC done
    //rc522_debug();
}

void rc522_clean_flags(void) {
    //Clear IRQ flags for timeout, error, RX
    //uint8_t value = ((1U << 0) | (1U << 1) | (1U << 5));
    rc522_write_reg(RC522_COMIRQ_REG, 0x7FU);
    rc522_write_reg(RC522_DIVIRQ_REG, 0x4U);
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
    rc522_modify_reg(RC522_T_MODE_REG, value, 0x8FU);

    //timebase_blocking_delay_ms(1000);
    //rc522_debug();
}

void rc522_config_txask(void) {
    //Forcing TxASK = 100. Reader not recognize card without it
    rc522_write_reg(RC522_TXASK_REG, 0x40);
}

void rc522_turn_on_antennas(void) {
    rc522_modify_reg(RC522_TXCONTROL_REG, 0x3, 0x3);
    //rc522_debug();
}

void rc522_config_mode(void) {
    //ModeReg

}

void rc522_config_crc(void) {
    rc522_modify_reg(RC522_MODE_REG, 0x1, 0x3);
}

void rc522_load_fifo(volatile uint8_t *fifo, uint8_t length) {
    rc522_write_reg(RC522_COMMAND_REG, RC522_COMMAND_IDLE); //Command = IDLE
    rc522_clean_flags();
    rc522_clean_fifo();
    for (int i = 0; i < length; i++) {
        rc522_write_reg(RC522_FIFODATA_REG, fifo[i]);
    }
}

void rc522_transcieve(volatile uint8_t *fifo, uint8_t length,
        uint8_t tx_last_bits) {
    rc522_load_fifo(fifo, length);

    //Command = transcieve
    rc522_write_reg(RC522_COMMAND_REG, RC522_COMMAND_TRANSCIEVE);
    //rc522_debug();

    //BITFRAMING
    uint8_t value = (tx_last_bits == 8) ? 0 : tx_last_bits; //value '0' is for 8
    value |= (1U << 7); //BITFRAMING.START_SEND
    rc522_write_reg(RC522_BITFRAMING_REG, value);

    rc522_debug();
}

uint8_t rc522_read_fifo(volatile uint8_t *fifo) {
    uint8_t length = rc522_read_reg(RC522_FIFOLEVEL_REG);

    for (int i = 0; i < length; i++) {
        fifo[i] = rc522_read_reg(RC522_FIFODATA_REG);
    }

    return length;
}














