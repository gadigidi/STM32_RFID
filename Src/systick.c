#include "systick.h"
#include "stm32f446xx.h"
#include <stdint.h>

void systick_init(void){
    SysTick->CTRL = 0; //timer off
    SysTick->CTRL |= ST_SRC_CLK; //timer off
    SysTick->LOAD = ST_INIT_VAL;
    SysTick->VAL = 0x0;
    SysTick->CTRL |= ST_ENABLE; //timer on
}

void systick_wait_tick(void){
    //Wait for ST_COUNTFLAG
    while (!(SysTick->CTRL & ST_COUNTFLAG)) {}
}

uint32_t systick_read_counter_val(void) {
    //Wait for ST_COUNTFLAG
    while (!(SysTick->CTRL & ST_COUNTFLAG)) {}
    //Read current counter value
    return SysTick->VAL;
}
