#ifndef TIM2_H_
#define TIM2_H_

#include <stdint.h>

#define APB1CLK				45000000U
//#define DIV					10000
#define PSC_VAL				(APB1CLK/10000U)-1
#define ARR_VAL				10000U-1

#define TIM2_CLK_EN			(1U<<0)
#define TIM2_CEN			(1U<<0)
#define TIM2_SR_UIF			(1U<<0)

void tim2_init(void);
void tim2_wait_tick(void);
void tim2_wait_tick(void);

#endif /* TIM2_H_ */
