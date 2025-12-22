#include "seg7.h"
#include "gpio.h"
#include "timebase.h"
#include "stm32f446xx.h"
#include <stdint.h>
#include <stdbool.h>

const uint32_t seg_bsrr[] = { 1610614752U, 1730150592U, 1149248352U,
		1174413792U, 1126180032U, 1111500192U, 1077946272U, 1728053472U,
		1073752032U, 1107305952U, 1090528992U, 1080043392U, 1623197472U,
		1142956992U, 1086334752U, 1103111712U, 1205870592U };

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
	seg7_select_digit(dig_bsrr[D]);
}

static uint8_t disp_nibble[4]; 
void seg7_update_buffer(uint16_t num) {
	disp_nibble[3] = ((num >> 0) & 0xFU);
	disp_nibble[2] = ((num >> 4) & 0xFU);
	disp_nibble[1] = ((num >> 8) & 0xFU);
	disp_nibble[0] = ((num >> 12) & 0xFU);
}

void seg7_auto_refresh(void) {
	static int curr_digit = 0;
	seg7_show_digit(curr_digit, disp_nibble[curr_digit]);
	curr_digit = (curr_digit + 1) % 4;
}

/*
 static volatile bool start_scroll_digits = 0;
void seg7_scroll_digits_enable(bool start1_stop0) {
	scroll_digits = start1_stop0;
}
 */

static uint8_t scroll_buffer[12];
void seg7_set_buffer_for_scroll(uint32_t num) {
	scroll_buffer[0] = ((num >> 0) & 0xFU);
	scroll_buffer[1] = ((num >> 4) & 0xFU);
	scroll_buffer[2] = ((num >> 8) & 0xFU);
	scroll_buffer[3] = ((num >> 12) & 0xFU);
	scroll_buffer[4] = ((num >> 16) & 0xFU);
	scroll_buffer[5] = ((num >> 20) & 0xFU);
	scroll_buffer[6] = ((num >> 24) & 0xFU);
	scroll_buffer[7] = ((num >> 28) & 0xFU);
	scroll_buffer[8] = 16U; //'-'
	scroll_buffer[9] = 16U; //'-'
	scroll_buffer[10] = 16U; //'-'
	scroll_buffer[11] = 16U; //'-'
}

static uint32_t scroll_start_time;
static bool scroll_enable = 0;
void seg7_enable_scroll(bool enable) {
	scroll_start_time = timebase_show_ms();
	scroll_enable = enable;
}

void seg7_scroll_digits(void) {

	static int curr_scroll_digit = 0;
	uint32_t time_now = timebase_show_ms();
	uint32_t time_delta = time_now - scroll_start_time;
	if ((scroll_enable) & (time_delta >= 1000)) {
		int nibbel2show;
		nibbel2show = (curr_scroll_digit) % 12;
		disp_nibble[3] = scroll_buffer[nibbel2show];
		nibbel2show = (curr_scroll_digit + 1) % 12;
		disp_nibble[2] = scroll_buffer[nibbel2show];
		nibbel2show = (curr_scroll_digit + 2) % 12;
		disp_nibble[1] = scroll_buffer[nibbel2show];
		nibbel2show = (curr_scroll_digit + 3) % 12;
		disp_nibble[0] = scroll_buffer[nibbel2show];

		curr_scroll_digit = (curr_scroll_digit + 1) % 12;
		scroll_start_time = timebase_show_ms();
	}
}









