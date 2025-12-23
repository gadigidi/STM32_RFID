#ifndef RC522_H_
#define RC522_H_

#include <stdint.h>
#include <stdbool.h>

#define RC522_WR            0
#define RC522_RD            1U
#define RC522_ADDR_MASK     0x7EU //b01111110 because lsb must be 0 and msb is wr/re

void rc522_init(void);
void rc522_hard_reset(void);
void rc522_write_reg(uint8_t addr, uint8_t data);
uint8_t rc522_read_reg(uint8_t addr);
void rc522_clean_irq(void);

#endif /* RC522_H_ */
