#include "rfid.h"
#include "rc522.h"
#include "rc522_debug.h"
#include "isr.h"
#include "timebase.h"
#include "spi.h"
#include "seg7.h"

#include "stm32f446xx.h"
#include <stdint.h>
#include <stdbool.h>

void rfid_init(void) {

    rfid_spi1_config();

    rc522_chip_init();

    /*
    if (!rc522_alive_status()) {
        rfid_state = RFID_ERROR;
    }
     */


    //Writing to TXCONTROL (antennas) is triggering the timer
    //So wait for it to finish before clean flags
    timebase_blocking_delay_ms(RFID_REQA_TIMEOUT_MS);
    rc522_clean_flags();
    //Enable interrupts from RFID_IRQ
    rfid_enable_irq();
}

void rfid_enable_irq(void) {
    //Enable clock to SYSCFG for EXTI configurations
    RCC->APB2ENR |= (1U << 14);
    //Select GPIOA as IRQ source for EXTI1, because the RC522 IRQ connected to PA1
    SYSCFG->EXTICR[0] &= ~(0xFU << 4);
    //SYSCFG->EXTICR[0] |= (1U << 4);
    //Clear mask for EXTI line 1
    EXTI->IMR |= (1U << 1);
    //Config IRQ sensetivity = active low (falling_dege only)
    EXTI->RTSR &= ~(1U << 1);
    EXTI->FTSR |= (1U << 1);

    //Clean old PR before anabling
    EXTI->PR = (1U << 1);

    isr_enable_interrupts(EXTI1_IRQn);
}

void rfid_spi1_config(void) {
    spi1_init();
    spi1_enable_clock();
    spi1_config_baudrate(SPI_BR_DIV64);
    spi1_config_mode(SPI_MASTER);
    spi1_config_clock_mode(SPI_MODE_0);
    spi1_config_bit_order(SPI_MSB_FIRST);
    spi1_config_nss(SPI_NSS_SOFTWARE);
    spi1_config_data_size(SPI_DATASIZE_8BIT);
    spi1_enable(1U);
}

static volatile bool rfid_pending_irq = 0;
void rfid_update_irq_flag(void) {
    rfid_pending_irq = 1;
    //comirqreg = rc522_read_reg(RC522_COMIRQ_REG);
    //rc522_clean_flags();
}

bool rfid_show_pending_irq_status(void) {
    return rfid_pending_irq;
}

typedef enum {
    RFID_IDLE, RFID_GET_ATQA, RFID_READ_UID, RFID_SHOW_UID, RFID_ERROR
} rfid_state_t;


static volatile uint8_t fifo[64];
void rfid_fsm(void) {
    static rfid_state_t rfid_state = RFID_IDLE; //Check later where to put
    int length;
    //uint8_t cascade_level;
    //static bool error_detected = 0;

    //Non blocking delays
    static uint32_t idle_start_time = 0;
    static uint32_t reqa_timeout_start_time = 0;
    //static uint32_t uid_timeout_start_time = 0;
    //static uint32_t cooldown_start_time = 0;
    uint32_t time_now;

    switch (rfid_state) {
    case RFID_IDLE:
        time_now = timebase_show_ms();
        uint8_t comien = rc522_read_reg(RC522_COMIEN_REG);
        uint32_t idle_delta_time = time_now - idle_start_time;
        if (idle_delta_time >= RFID_POLL_PERIOD_MS) {
            rc522_clean_flags();
            rfid_pending_irq = 0; //Just to be sure before waiting for new irq
            rc522_send_reqa();
            //rc522_debug();
            reqa_timeout_start_time = timebase_show_ms();
            rfid_state = RFID_GET_ATQA;
        }
        break;
    case RFID_GET_ATQA:
        time_now = timebase_show_ms();
        uint32_t reqa_delta_time = time_now - reqa_timeout_start_time;
        //rc522_debug();
        if (reqa_delta_time >= RFID_REQA_TIMEOUT_MS) {
            idle_start_time = timebase_show_ms();
            //rc522_debug();
            rfid_state = RFID_IDLE;
        }
        if (rfid_pending_irq) {
            uint8_t comirqreg = rc522_read_reg(RC522_COMIRQ_REG);
            rc522_clean_flags();
            rfid_pending_irq = 0;
            //rc522_debug();
            if (comirqreg & 0x1U) {
                //uint8_t error = rc522_read_reg(RC522_ERROR_REG);
                idle_start_time = timebase_show_ms();
                //rc522_debug();
                rfid_state = RFID_IDLE;
            } else {
                rc522_get_atqa(fifo);
                uint8_t fifo0 = fifo[0];
                uint8_t fifo1 = fifo[1];
                //reqa_timeout_start_time = timebase_show_ms();
                rfid_state = RFID_READ_UID;
            }
        }
        break;
    case RFID_READ_UID:
        //error_detected = rfid_check_error();
        //if (error_detected) {
        //rfid_state = RFID_ERROR;
        //}
        //else {
            //rc522_read_uid();
            rfid_state = RFID_SHOW_UID;
        //}
        break;
    case RFID_SHOW_UID:
        //seg7_set_buffer_for_scroll(uid);
        rfid_state = RFID_ERROR;
        break;
    case RFID_ERROR:
        //rfid_error(); //TODO
        rfid_state = RFID_IDLE;
    }
}










