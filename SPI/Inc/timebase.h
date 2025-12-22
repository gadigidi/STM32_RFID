#ifndef TIMBASE_H_
#define TIMBASE_H_

#include <stdint.h>
#include <stdbool.h>

void timebase_init(void);
uint32_t timebase_show_ms(void);
bool timebase_nonblocking_delay_ms(uint32_t t_start, uint32_t delay_ms);

#endif /* TIMBASE_H_ */
