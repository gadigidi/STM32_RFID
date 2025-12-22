#include "rfid.h"
#include "gpio.h"
#include "isr.h"
#include "timebase.h"
#include "spi.h"
#include "stm32f446xx.h"
#include <stdint.h>
#include <stdbool.h>

void rfid_init(void) {
    gpio_init(GPIOAEN);

    //Set PA1 for RFID_IRQ input
    gpio_pin_mode(GPIOA, RFID_IRQ_PIN, GPIO_MODE_INPUT);

    //Enable interrupts from RFID_IRQ
    enable_interrupts(EXTI1_IRQn);

}

void spi1_config_for_rfid(void) {
    spi1_init();
    spi1_config_baudrate(SPI_BR_DIV64);
    spi1_enable_clock();
    spi1_config_mode(SPI_MASTER);
    spi1_config_clock_mode(SPI_NSS_SOFTWARE);
    spi1_config_bit_order(SPI_MSB_FIRST);
    spi1_config_nss(SPI_NSS_SOFTWARE);
    spi1_config_data_size(SPI_DATASIZE_8BIT);
    spi1_enable(1U);
}

static bool rfid_pending_irq = 0;
void rfid_update_irq(bool val) {
    rfid_pending_irq = val;
}

typedef enum {
    RFID_IDLE,
    RFID_WAIT_IRQ,
    RFID_REQA,
    //RFID_ANTICOLL,
    RFID_READ_UID,
//RFID_ERROR
} rfid_state_t;

static rfid_state_t rfid_state = RFID_IDLE; //Check later where to put
void rfid_fsm(void) {
    switch (rfid_state) {
    case RFID_IDLE:
        if (rfid_pending_irq) {
            rfid_state = RFID_WAIT_IRQ;
            rfid_pending_irq = 0;
        }
        break;
    case RFID_WAIT_IRQ:
        if (rfid_pending_irq) {
            rfid_state = RFID_REQA;
            rfid_pending_irq = 0;
        }
        break;
    case RFID_REQA:
        if (rfid_pending_irq) {
            uint32_t uid = spi1_read();
            rfid_state = RFID_READ_UID;
            rfid_pending_irq = 0;
        }
        break;
    case RFID_READ_UID:
        if (rfid_pending_irq) {
            seg7_set_buffer_for_scroll();
            rfid_state = RFID_IDLE;
            rfid_pending_irq = 0;
        }
        break;
    }
}

