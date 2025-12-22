#include "stm32f446xx.h"
#include <stdint.h>
#include "tim2.h"
#include "timebase.h"
#include "timebase_isr.h"
#include "seg7.h"

void enable_interrupts(int irqn) {
	int reg = irqn / 32;
	int offset = irqn % 32;
	NVIC->ISER[reg] |= (1U << offset);
}

void TIM2_IRQHandler(void) {
	timebase_increase_ms();
	TIM2->SR &= ~TIM2_SR_UIF;

	//Auto refresh for the 7-segments screen
	seg7_auto_refresh();
}

