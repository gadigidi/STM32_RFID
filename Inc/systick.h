#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

#define SYSCLK                    16000000
#define ST_INIT_VAL                (SYSCLK/1000)-1
#define ST_SRC_CLK                (1U<<2)
#define ST_COUNTFLAG            (1U<<16)
#define ST_ENABLE                (1U<<0)



void systick_init(void);
void systick_wait_tick(void);
uint32_t systick_read_counter_val(void);

#endif /* SYSTICK_H_ */
