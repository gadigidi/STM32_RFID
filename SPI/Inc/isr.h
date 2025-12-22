#ifndef ISR_H_
#define ISR_H_

#include <stdint.h>

void enable_interrupts(int irqn);
void TIM2_IRQHandler(void);

#endif /* ISR_H_ */
