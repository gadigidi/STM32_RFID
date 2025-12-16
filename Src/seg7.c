#include "seg7.h"
#include "gpio.h"
#include "stm32f446xx.h"
#include <stdint.h>

const uint32_t seg_bsrr[] = { 1610614752U, 1730150592U, 1149248352U,
		1174413792U, 1126180032U, 1111500192U, 1077946272U, 1728053472U,
		1073752032U, 1107305952U };

const uint32_t dig_bsrr[] = { 65550U, 131085U, 262155U, 524295U };

void seg7_init(void){
	gpio_init(GPIOBEN);
	gpio_pin_mode(GPIOB, SEG_A_PIN, GPIO_MODE_OUTPUT);
	gpio_pin_mode(GPIOB, SEG_B_PIN, GPIO_MODE_OUTPUT);
	gpio_pin_mode(GPIOB, SEG_C_PIN, GPIO_MODE_OUTPUT);
	gpio_pin_mode(GPIOB, SEG_D_PIN, GPIO_MODE_OUTPUT);
	gpio_pin_mode(GPIOB, SEG_E_PIN, GPIO_MODE_OUTPUT);
	gpio_pin_mode(GPIOB, SEG_F_PIN,GPIO_MODE_OUTPUT );
	gpio_pin_mode(GPIOB, SEG_G_PIN, GPIO_MODE_OUTPUT);
	gpio_pin_mode(GPIOB, SEG_DEC_PIN, GPIO_MODE_OUTPUT);

	gpio_init(GPIOCEN);
	gpio_pin_mode(GPIOC, DIG1_PIN, GPIO_MODE_OUTPUT);
	gpio_pin_mode(GPIOC, DIG2_PIN, GPIO_MODE_OUTPUT);
	gpio_pin_mode(GPIOC, DIG3_PIN, GPIO_MODE_OUTPUT);
	gpio_pin_mode(GPIOC, DIG4_PIN, GPIO_MODE_OUTPUT);
}

void seg7_select_digit(uint32_t bsrr) {
	GPIOC->BSRR = bsrr;
}

void seg7_deselect_all_digits(void) {
	GPIOC->BSRR = SEG7_BSRR_DIGS_OFF;
}

void seg7_select_seg(uint32_t bsrr) {
	GPIOB->BSRR = bsrr;
}

void seg7_show_digit(int D, int digit) {
	seg7_deselect_all_digits();
	seg7_select_seg(seg_bsrr[digit]);
	seg7_select_digit(dig_bsrr[D - 1]); //DIGs are defined as D1 D2 D3 D4
}
