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

static bool fatal_error_detected = 0;
static bool error_detected = 0;
static bool read_uid_completed = 0;
void rfid_init(void) {

    rfid_spi1_config();

    fatal_error_detected = rc522_chip_init(); //Return 1 if chip is not alive

    //Writing to TXCONTROL (antennas) is triggering the timer
    //So wait for it to finish before clean flags
    timebase_blocking_delay_ms(RFID_REQA_TIMEOUT_MS);
    rc522_clean_flags();

    //Enable interrupts from RFID_IRQ
    rfid_enable_irq();
}

void rfid_enable_irq(void) {

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
    spi1_enable(1);
}

static volatile bool rfid_pending_irq = 0;
void rfid_update_irq_flag(void) {
    rfid_pending_irq = 1;
}

bool rfid_show_pending_irq_status(void) {
    return rfid_pending_irq;
}

typedef enum {
    RFID_PRE_IDLE,
    RFID_IDLE,
    RFID_SEND_REQA,
    RFID_WAIT_ATQA,
    RFID_SEND_ANTICOL_CL1,
    RFID_WAIT_ANTICOL_CL1,
    RFID_CALC_CRC_CL1,
    RFID_WAIT_CRC_CL1_RESULT,
    RFID_SEND_SELECT_CL1,
    RFID_WAIT_SAK,
    RFID_UID_STATUS,
    RFID_SHOW_UID_TEXT,
    RFID_SHOW_UID_DIGITS,
    RFID_SHOW_ERROR,
    RFID_FATAL_ERROR
} rfid_state_t;

static volatile uint8_t uid[4];
static volatile uint8_t bcc1_recieved;
static volatile uint8_t crc_msb;
static volatile uint8_t crc_lsb;
//static volatile uint8_t uid_is_long;
static volatile int length;

static volatile uint8_t fifo[64];
void rfid_send_reqa(void) {
    fifo[0] = 0x26 & 0x7FU; //Send REQA
    rc522_transcieve(fifo, 1, 7);
    uint8_t fifo_level = rc522_read_reg(RC522_FIFOLEVEL_REG);
}

void rfid_get_atqa(void) {
    uint8_t length = rc522_read_fifo(fifo);
    if (length != 2) {
        error_detected = 1;
    }
}

void rfid_send_anticol_cl1(void) {
    fifo[0] = 0x93;
    fifo[1] = 0x20;
    rc522_transcieve(fifo, 2, 0);
}

bool rfid_check_bcc(volatile uint8_t *array, int length, uint8_t bcc_recieved) {
    uint8_t bcc_expected = 0;
    for (int i = 0; i < length; i++) {
        bcc_expected ^= array[i];
    }
    return (bcc_recieved == bcc_expected);
}

void rfid_calc_crc_cl1() {
    fifo[0] = 0x93;
    fifo[1] = 0x70;
    fifo[2] = uid[0];
    fifo[3] = uid[1];
    fifo[4] = uid[2];
    fifo[5] = uid[3];
    fifo[6] = bcc1_recieved;
    rc522_load_fifo(fifo, 7);
    rc522_write_reg(RC522_BITFRAMING_REG, 0);
    rc522_write_reg(RC522_COMMAND_REG, RC522_COMMAND_CRC_CALC);
}

void rfid_send_select_cl1(void) {
    fifo[7] = crc_lsb;
    fifo[8] = crc_msb;

    rc522_transcieve(fifo, 9, 0);
}

void rfid_show_uid_text(void) {
    seg7_set_blinking_text(SEG7_UID);
    seg7_set_fsm_state(SEG7_BLINKING_ANIMATION);
}

void rfid_show_uid_digits(void) {
    seg7_set_fsm_state(SEG7_OFF);
    seg7_set_buffer_for_scroll(uid);
    seg7_set_fsm_state(SEG7_SCROLL);
}

void rfid_show_error(void) {
    seg7_set_blinking_text(SEG7_ERROR);
    seg7_set_fsm_state(SEG7_BLINKING_ANIMATION);
}

static rfid_state_t rfid_state = RFID_IDLE;
static uint32_t timeout_start_time = 0;
void rfid_non_blocking_timeout(uint32_t timeout_ms, rfid_state_t next_state) {
    //static uint32_t timeout_start_time = 0;
    static uint32_t time_now;
    static uint32_t delta_time;
    time_now = timebase_show_ms();
    delta_time = time_now - timeout_start_time;
    if (delta_time >= timeout_ms) {
        if (next_state == RFID_PRE_IDLE) {
            seg7_set_fsm_state(SEG7_IDLE_ANIMATION);
        }
        if (next_state == RFID_SHOW_UID_DIGITS) {
            rfid_show_uid_digits();
        }
        rfid_state = next_state;
        timeout_start_time = timebase_show_ms();
    }
}

void rfid_fsm_reset(void) {
    if (fatal_error_detected) {
        rfid_show_error();
        rfid_state = RFID_FATAL_ERROR;
    }
    if (error_detected) {
        rfid_show_error();
        rfid_state = RFID_SHOW_ERROR;
    }
    else {
        rfid_state = RFID_IDLE;
    }
}

void rfid_fsm(void) {

    switch (rfid_state) {

    case RFID_PRE_IDLE: {
        read_uid_completed = 0;
        error_detected = 0;
        rfid_state = RFID_IDLE;
        timeout_start_time = timebase_show_ms();
        break;
    }

    case RFID_IDLE: {
        rfid_non_blocking_timeout(RFID_POLL_PERIOD_MS, RFID_SEND_REQA);
        break;
    }

    case RFID_SEND_REQA: {
        rfid_send_reqa();
        rfid_state = RFID_WAIT_ATQA;
        timeout_start_time = timebase_show_ms();
        break;
    }

    case RFID_WAIT_ATQA: {
        rfid_non_blocking_timeout(RFID_REQA_TIMEOUT_MS, RFID_SHOW_ERROR);
        if (rfid_pending_irq) {
            uint8_t comirqreg = rc522_read_reg(RC522_COMIRQ_REG);
            rc522_clean_flags();
            rfid_pending_irq = 0;
            //rc522_debug();
            if (comirqreg & 0x1U) { //Timeout IRQ
                rfid_state = RFID_IDLE;
                timeout_start_time = timebase_show_ms();
            } else if (comirqreg & (0x1 << 5)) { //Rx IRQ
                rfid_get_atqa();
                rfid_state = RFID_SEND_ANTICOL_CL1;
                timeout_start_time = timebase_show_ms();
            }
        }
        break;
    }

    case RFID_SEND_ANTICOL_CL1: {

        rfid_send_anticol_cl1();
        rc522_debug();
        rfid_state = RFID_WAIT_ANTICOL_CL1;
        timeout_start_time = timebase_show_ms();
        break;
    }

    case RFID_WAIT_ANTICOL_CL1: {
        rfid_non_blocking_timeout(RFID_UID_TIMEOUT_MS, RFID_SHOW_ERROR);

        if (rfid_pending_irq) {
            uint8_t comirqreg = rc522_read_reg(RC522_COMIRQ_REG);
            rc522_clean_flags();
            rfid_pending_irq = 0;
            //rc522_debug();
            if (comirqreg & 0x1) { //Timeout IRQ
                //rc522_debug();
                rfid_state = RFID_SHOW_ERROR;
                timeout_start_time = timebase_show_ms();
            } else if (comirqreg & (0x1 << 5)) { //Rx IRQ
                length = rc522_read_fifo(fifo);
                uid[0] = fifo[0];
                uid[1] = fifo[1];
                uid[2] = fifo[2];
                uid[3] = fifo[3];
                bcc1_recieved = fifo[4];

                bool bcc_okay = rfid_check_bcc(uid, 4, bcc1_recieved);
                if (!bcc_okay) {
                    rfid_show_error();
                    rfid_state = RFID_SHOW_ERROR;
                    timeout_start_time = timebase_show_ms();
                }

                rfid_state = RFID_CALC_CRC_CL1;
                timeout_start_time = timebase_show_ms();
            }
        }
        break;
    }

    case RFID_CALC_CRC_CL1: {
        rfid_calc_crc_cl1();
        rfid_state = RFID_WAIT_CRC_CL1_RESULT;
        timeout_start_time = timebase_show_ms();
        break;
    }

    case RFID_WAIT_CRC_CL1_RESULT: {
        rfid_non_blocking_timeout(RFID_CRC_TIMEOUT_MS, RFID_SHOW_ERROR);
        if (rfid_pending_irq) {
            uint8_t divirqreg = rc522_read_reg(RC522_DIVIRQ_REG);
            rc522_clean_flags();
            rfid_pending_irq = 0;
            //rc522_debug();
            if (divirqreg & 0x4) { //CRC IRQ
                crc_msb = rc522_read_reg(RC522_CRC_MSB_REG);
                crc_lsb = rc522_read_reg(RC522_CRC_LSB_REG);

                //rc522_debug();
                rfid_state = RFID_SEND_SELECT_CL1;
                timeout_start_time = timebase_show_ms();
            }
        }
        break;
    }

    case RFID_SEND_SELECT_CL1: {
        rfid_send_select_cl1();
        //rc522_debug();
        rfid_state = RFID_WAIT_SAK;
        timeout_start_time = timebase_show_ms();
        break;
    }

    case RFID_WAIT_SAK: {
        rfid_non_blocking_timeout(RFID_CRC_TIMEOUT_MS, RFID_SHOW_ERROR);
        if (rfid_pending_irq) {
            uint8_t comirqreg = rc522_read_reg(RC522_COMIRQ_REG);
            rc522_clean_flags();
            rfid_pending_irq = 0;
            //rc522_debug();
            if (comirqreg & 0x4) { //CRC IRQ
                uint8_t fifolevel = rc522_read_reg(RC522_FIFOLEVEL_REG);
                int length = rc522_read_fifo(fifo);
                if (length != 3) {
                    error_detected = 1;
                }
                else {
                    uint8_t sak = fifo[0];
                    if ((sak & 0x4) == 0) {
                        read_uid_completed = 1;
                    }
                    else {
                        error_detected = 1;
                    }
                }

                //rc522_debug();
                rfid_state = RFID_UID_STATUS;
                timeout_start_time = timebase_show_ms();
            }
        }
        break;
    }
    
    case RFID_UID_STATUS: {
        if (error_detected) {
            rfid_show_error();
            rfid_state = RFID_SHOW_ERROR;
        }
        if (fatal_error_detected) {
            rfid_show_error();
            rfid_state = RFID_FATAL_ERROR;
        }
        if (read_uid_completed) {
            rfid_show_uid_text();
            rfid_state = RFID_SHOW_UID_TEXT;
        }
        timeout_start_time = timebase_show_ms();
        break;
    }

    case RFID_SHOW_UID_TEXT: {
        rfid_non_blocking_timeout(RFID_SHOW_STATUS_TIMEOUT_MS,
                RFID_SHOW_UID_DIGITS);
        break;
    }

    case RFID_SHOW_UID_DIGITS:
    {
        rfid_non_blocking_timeout(RFID_SHOW_UID_TIMEOUT_MS, RFID_PRE_IDLE);
        break;
    }

    case RFID_SHOW_ERROR: {
        rfid_non_blocking_timeout(RFID_SHOW_STATUS_TIMEOUT_MS,
                    RFID_PRE_IDLE);
        break;
    }

    case RFID_FATAL_ERROR: {
        break;
    }

    }            //switch
}            //rfid_fsm










