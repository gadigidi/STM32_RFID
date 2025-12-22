#ifndef SEG7_H_
#define SEG7_H_

#include <stdint.h>

//All segments on GPIOB
#define SEG_A_PIN				5U
#define SEG_B_PIN				6U
#define SEG_C_PIN				7U
#define SEG_D_PIN				8U
#define SEG_E_PIN				9U
#define SEG_F_PIN				10U
#define SEG_G_PIN				13U
#define SEG_DEC_PIN				14U

//All digits on GPIOC
#define DIG1_PIN				0
#define DIG2_PIN				1U
#define DIG3_PIN				2U
#define DIG4_PIN				3U

#define SEG7_SEG_ACTIVE			0
#define SEG7_DIG_ACTIVE			1U

extern const uint32_t seg_bsrr[16];
extern const uint32_t dig_bsrr[4];
#define SEG7_BSRR_DIGS_OFF		15U

void seg7_init (void);
void seg7_select_digit(uint32_t bsrr);
void seg7_deselect_all_digits(void);
void seg7_select_seg(uint32_t bsrr);
void seg7_show_digit(int D, int digit);
void seg7_update_buffer(uint16_t num);
void seg7_auto_refresh(void);

#endif /* SEG7_H_ */
