#include "rc522_ports.h"
#include "spi.h"
#include "gpio.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

void rc522_ports_init(void) {
    gpio_init(GPIOAEN);

    //set IRQ as input
    gpio_pin_mode(GPIOA, RC522_IRQ_PIN, GPIO_MODE_INPUT);

    //set CS as output
    gpio_pin_mode(GPIOA, RC522_CS_PIN, GPIO_MODE_OUTPUT);
    gpio_pin_pupdr(GPIOA, RC522_CS_PIN); //Add internal pull up resistor

    //Add internal pull up resistor for MISO pin
    gpio_pin_pupdr(GPIOA, RC522_MISO_PIN);

    //set RST as output
    gpio_pin_mode(GPIOA, RC522_RST_PIN, GPIO_MODE_OUTPUT);
    //Add internal pull up resistor for RST pin
    //gpio_pin_pupdr(GPIOA, RC522_RST_PIN); //Actually external pull up resistor added
}

void rc522_assert_reset(void) {
    GPIOA->BSRR = RC522_RST_LOW_BSRR;
}

void rc522_deassert_reset(void) {
    GPIOA->BSRR = RC522_RST_HIGH_BSRR;
}

void rc522_assert_cs(void) {
    GPIOA->BSRR = RC522_CS_LOW_BSRR;
}

void rc522_deassert_cs(void) {
    GPIOA->BSRR = RC522_CS_HIGH_BSRR;
}

